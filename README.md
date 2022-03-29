# PulsarBash
This has **not** been finished yet. Sorrry but I will finish as soon as I can.

The OS I am using is Ubuntu 18.04.

## Goal one
<details>
  <summary> Click to expand: A bash script</summary>
  <br>
  Please refer to the bash script /src/exclamation.sh in the repo. It adds an exclamation at the end of the input.  
 
</details>

## Goal two
<details>
  <summary> Click to expand: A bash runtime</summary>
 <br>
  The C++ code is straightforward: it consumes data from one pulsar topic, then call the `system()` function to execute the bash, and finially return the bash script result to another pulsar topic. If you have dependencies [installed](https://pulsar.apache.org/docs/en/client-libraries-cpp/), you can compile the pulsar c++ client code and generate executable `bashruntime` by:

`g++ ./bashruntime.cc -o bashruntime /usr/lib/libpulsar.so -I ../include`

If you have [standalone Pulsar](https://pulsar.apache.org/docs/en/standalone/#start-pulsar-standalone) running on your local machine, you can then run `./bashruntime localhost` where `localhost` is the Pulsar url to see the that we have successfully called the bash script to add an exclamation and pulish it to another topic. 
  
If you cannot install the C++ client dependencies, you can call me, or I provided a docker image of this bashruntime, so you do not need to worry about the indepencies and compiling anymore.  
  
 `docker pull wangzhen1997/pulsar_bash:1`
  
`docker run --network="host" -it --rm -it wangzhen1997/pulsar_bash:1 localhost`
  
  Here I add `--network="host" ` because the standalone Pulsar is running on your host machine rather than contianer itself, otherwise the localhost of the host machine is not reachable from the container. 
  
  If you see the messages "it is a good day my friends" ends with a ! from the terminal, you have it successfully run. 
  
  ![image](https://user-images.githubusercontent.com/45562036/160527328-2c1a1167-326e-4200-a21d-c9baeb028ce5.png)

  
  In the next part, I will cover how can you run the Pulsar in Kubernetes cluster, so your container can directly talk with Pulsar using Pulsar proxy service url.  
 
</details>

## Goal three 
<details>
  <summary> Click to expand: Fuction as image and StatefulSet</summary>
  <br>
   <b>Docker image</b>
   Please refer to the `Dockerfile` in the repo for details on building a Docker image for this bashruntime: bascially you choose the approriate base
   image, and then COPY the execuable together with dependience and libraies to the image. I kept it in mind that the image should be small. It is pushed    to [DockerHub](https://hub.docker.com/r/wangzhen1997/pulsar_bash), so you can give it a try.  
  <br><br>
  <b>Deploy the StatefulSet on Kubernetes</b> 
  For this part, I except you to have a Pulsar deployed on Kubernetes first, so if you do not have a Kubernetes cluster installed, you can try this [k8sinstall-kubeadm](https://github.com/cncamp/101/blob/master/k8s-install/k8s-by-kubeadm/3.k8s-install.md). They you can follow [this](https://pulsar.apache.org/docs/en/kubernetes-helm/#step-1-install-pulsar-helm-chart) to install Pulsar on Kubernetes. Simply copy and paste all 1-6 in step 1 and wait for a few mins to get the Pulsar service ready. Please Do NOT use minikube because it brought me many problems. 
 
  If you run this and see pulsar-mini-proxy service has an CLUSTER-IP then your pods within this cluster can talk with Pulsar using this CLUSTER-IP. To get an EXTERNAL-IP, you might need to try EKS which provides you load-balancer, but CLUSTER-IP is enough for our case. 
  
  ` kubectl get service -n pulsar`
  ![image](https://user-images.githubusercontent.com/45562036/160528277-0da91937-84ac-41f0-a339-6082e81782eb.png)
 
  
</details>

 
Next, we show how to run a Pulsar cluster on kubernetes, and let the bash function deployment to communicate with this kubernets Pulsar instead of local Pulsar binary.  
