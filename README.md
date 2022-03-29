# PulsarBash
This has **not** been finished yet. Sorrry but I will finish as soon as I can.

Currently I can run a standalone pulsar broker on my local machine, and then run my c++ executable to interact with the broker to finish the task requirements.

## Goal one
<details>
  <summary> Click to expand: A bash script</summary>
  <br>
  Please refer to the bash script `/src/exclamation.sh` It adds an exclamation at the end of the input.  
 
</details>

## Goal two
<details>
  <summary> Click to expand: A bash runtime</summary>
 <br>
  The C++ code is straightforward: it consumes data from one pulsar topic, then call the `system()` function to execute the bash, and finially return the bash script result to another pulsar topic. If you have dependencies [installed](https://pulsar.apache.org/docs/en/client-libraries-cpp/), you can compile the pulsar c++ client code by:

`g++ ./bashruntime.cc -o bashruntime /usr/lib/libpulsar.so -I ../include`

If you have [standalone Pulsar](https://pulsar.apache.org/docs/en/standalone/#start-pulsar-standalone) running on your local machine, you can then run `./bashruntime localhost` where `localhost` is the Pulsar url to see the that we have successfully called the bash script to add an exclamation and pulish it to another topic. If you cannot install the dependencies, you can call me, or jump to Goal three, as I provided a docker image of this bashruntime, so you do not need to worry about the indepencies and compiling. 
 
 I also built a docker image using the `Dockerfile` (executable together with the dependencies) uploaded in the repo, and pushed it to [Dockerhub](https://hub.docker.com/r/wangzhen1997/pulsar_bash)  You can try 

`docker run -it --rm wangzhen1997/pulsar_bash:1` 
 
</details>



<!-- **But** you will get broker connection error, because the `pulsar_bash` runs inside a container and it cannot communicate with host network. One solution is to run a `broker` background process along with the `bash_runtime` process in one container, but I think this is not scalable and practical. I am having diffciluity on deploying this [pulsar-standalone](https://hub.docker.com/r/apachepulsar/pulsar-standalone) on k8s. If succesfully, I can expose it as a service and then my `bash_runtime` can talk with broker ...
 -->
 
Next, we show how to run a Pulsar cluster on kubernetes, and let the bash function deployment to communicate with this kubernets Pulsar instead of local Pulsar binary.  
