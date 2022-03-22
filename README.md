# PulsarBash
This has **not** been finished yet. Sorrry but I will finish as soon as I can.

Currently I can run a standalone pulsar broker on my local machine, and then run my c++ executable to interact with the broker to finish the task requirements.

The C++ code is straightforward: it consumes data from one pulsar topic, then call the `system()` function to execute the bash, and finially return the bash script result to another pulsar topic. If you have dependencies installed, you can compile the pulsar c++ client code by:

`g++ ./bashruntime.cc -o bashruntime /usr/lib/libpulsar.so -I ../include`

I also built a docker image using the `Dockerfile` (executable together with the dependencies) uploaded in the repo, and pushed it to [Dockerhub](https://hub.docker.com/r/wangzhen1997/pulsar_bash)  You can try 

`docker run -it --rm wangzhen1997/pulsar_bash:1` 

**But** you will get broker connection error, because the `pulsar_bash` runs inside a container and it cannot communicate with host network. One solution is to run a `broker` background process along with the `bash_runtime` process in one container, but I think this is not scalable and practical. I am having diffciluity on deploying this [pulsar-standalone](https://hub.docker.com/r/apachepulsar/pulsar-standalone) on k8s. If succesfully, I can expose it as a service and then my `bash_runtime` can talk with broker ...
