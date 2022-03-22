#include <pulsar/Client.h>
#include <thread>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>

// g++ ./bashruntime.cc -o bashruntime /usr/lib/libpulsar.so -I ../include
// docker build . -t pulsar_bash:1 -f /home/zhen/Desktop/job/streamnative/task/Dockerfile
// docker run -it --rm -it pulsar_bash:1 
using namespace pulsar;

#define PREFIX "persistent://public/default/"
#define BASH_SCRIPT_PATH "exclamation.sh"
#define BROKER "pulsar://host.docker.internal:6650"
// #define BASH_SCRIPT_PATH "/home/zhen/Desktop/job/streamnative/task/src/exclamation.sh"

int produceSth(std::string userTopic, std::string content) {
    Client client(BROKER);
    Producer producer;
    std::string topic = PREFIX + userTopic;
    Result result = client.createProducer(topic, producer);
    if (result != ResultOk) {
        std::cout << "Error creating producer: " << result << std::endl;
        return -1;
    }

    Message msg = MessageBuilder().setContent(content).setProperty("x", "1").build();
    result = producer.send(msg);
    if (result != ResultOk) {
        std::cout << "The message "
                  << " could not be sent, received code: " << result << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << content[content.length() - 1] << std::endl << std::endl;
    if (content[content.length() - 2] == '!') {
        std::cout << "************************************************************************************"
                  << std::endl;
        std::cout << "Woohoo! Successfully passed the bash output - " << content << "to topic - " << userTopic
                  << std::endl;
        std::cout << "************************************************************************************"
                  << std::endl;
        std::cout << std::endl;
    }

    client.close();
    return 0;
}

Message thenConsume(std::string userTopic) {
    Client client("pulsar://localhost:6650");
    Consumer consumer;
    ConsumerConfiguration config;
    config.setSubscriptionInitialPosition(InitialPositionEarliest);
    std::string topic = PREFIX + userTopic;
    Result result = client.subscribe(topic, "consumer-1", config, consumer);
    if (result != ResultOk) {
        std::cout << "Failed to subscribe!! : " << result << std::endl;
        exit(0);
    }

    Message msg;
    consumer.receive(msg);
    std::cout << "Received: " << msg << "  with payload" << std::endl
              << "------------------" << msg.getDataAsString() << std::endl;

    consumer.acknowledge(msg);

    std::cout << "Finished consuming synchronously!" << std::endl;

    client.close();
    return msg;
}

std::string bash(Message msgGotten, std::string bashPath) {
    // std::string temp = "sudo ";
    std::string command = "sh " + bashPath + " " + msgGotten.getDataAsString();
    /*
        redirect bash result from stdout to a file /tmp/bash_output
    */
    int o = dup(fileno(stdout));
    freopen("/tmp/bash_output", "w+", stdout);
    system(command.c_str());
    dup2(o, fileno(stdout));
    close(o);

    /*
        read bash_output
    */
    std::ifstream inFile("/tmp/bash_output");
    std::ostringstream buf;
    buf << inFile.rdbuf();
    inFile.close();

    //std::cout << "LOL " << buf.str() << std::endl << std::endl << std::endl;
    return buf.str();
}
int main() {
    produceSth("topicForConsume",
               "It is a good day my friends");  // mimic there is an existing topic in broker

    // step 1, consume data from one pulsar topic
    Message msgGottten = thenConsume("topicForConsume");

    // step 2, call bash script to process the data just consumed from pulsar
    std::string bash_output = bash(msgGottten, BASH_SCRIPT_PATH);

    // step 3, pass the "bash_output" to another topic
    // std::cout<<bash_output <<std::endl;
    produceSth("anotherTopic", bash_output);

    // step 4, verify we can get bash_output from topic "anotherTopic"
    std::string returnFromAnotherTopic = thenConsume("anotherTopic").getDataAsString();
    std::cout << "After passing bash output to anotherTopic, we can consume and get: "
              << returnFromAnotherTopic << std::endl;
    return 0;
}
