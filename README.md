# PulsarBash

The OS I am using is Ubuntu 18.04.

## Goal one
<details>
  <summary> Click to expand: A bash script</summary>
  <br>
  
  Please refer to the bash script `/src/exclamation.sh` in the repo. It adds an exclamation at the end of the input.  
 
</details>

## Goal two
<details>
  <summary> Click to expand: A bash runtime</summary>
 <br>
  The C++ code is straightforward: it consumes data from one pulsar topic, then call the `system()` function to execute the bash, and finially return the
  bash script result to another pulsar topic. 
  
  If you have [dependencies](https://pulsar.apache.org/docs/en/client-libraries-cpp/) installed, 
  you can compile the pulsar c++ client code and generate executable `bashruntime` by:

`g++ ./bashruntime.cc -o bashruntime /usr/lib/libpulsar.so -I ../include`

If you have [standalone Pulsar](https://pulsar.apache.org/docs/en/standalone/#start-pulsar-standalone) running on your local machine, you can then run `./bashruntime localhost` where `localhost` is the Pulsar url to see the that we have successfully called the bash script to add an exclamation and pulish it to another topic. 
  
If you cannot install the C++ client dependencies, that is okay. I provided a docker image of this bashruntime, so you do not need to worry about the indepencies and compiling anymore.  
  
 `docker pull wangzhen1997/pulsarbash:1`
  
`docker run --network="host" -it --rm -it wangzhen1997/pulsarbash:1 localhost`
  
  Here I add `--network="host" ` because the standalone Pulsar is running on your host machine rather than the contianer itself, otherwise the localhost of the host machine is not reachable from the container. 
  
  If you see the messages "it is a good day my friends" ends with a ! from the terminal, you have it successfully run! 
  
  ![image](https://user-images.githubusercontent.com/45562036/160527328-2c1a1167-326e-4200-a21d-c9baeb028ce5.png)

  
  In the next part, I will cover how can you run the Pulsar in Kubernetes cluster, so your container can directly talk with Pulsar using Pulsar proxy service url.  
 
</details>

## Goal three 
<details>
  <summary> Click to expand: Fuction as image and StatefulSet</summary>
  
  <br>
  
   <b>Docker image</b>
  
   Please refer to the `Dockerfile` in the repo for details on building a Docker image for this bashruntime: bascially you choose the approriate base
   image, and then COPY the execuable together with dependience and libraies to the image. I kept it in mind that the image should be small. It is pushed    to [DockerHub](https://hub.docker.com/r/wangzhen1997/pulsarbash), so you can give it a try. 
  
  <br>
  
  <b>Deploy the StatefulSet on Kubernetes</b> 
  
  For this part, I except you to have a Pulsar deployed on Kubernetes first, so if you do not have a Kubernetes cluster installed, you can try this [k8sinstall-kubeadm](https://github.com/cncamp/101/blob/master/k8s-install/k8s-by-kubeadm/3.k8s-install.md). They you can follow [this](https://pulsar.apache.org/docs/en/kubernetes-helm/#step-1-install-pulsar-helm-chart) to install Pulsar on Kubernetes. Simply copy and paste all 1-6 in step 1 and wait for a few mins to get the Pulsar service ready. Please Do NOT use minikube because it brought me many problems. 
 
  If you try get service and see pulsar-mini-proxy service has an CLUSTER-IP then your pods within this cluster can talk with Pulsar using this IP. To get an EXTERNAL-IP, you might need to try EKS which provides you a real load-balancer, but CLUSTER-IP is enough for our case. 
  
  ` kubectl get service -n pulsar`
  ![image](https://user-images.githubusercontent.com/45562036/160528277-0da91937-84ac-41f0-a339-6082e81782eb.png)
 
  For me, the service pulsar-mini-proxy IP is `10.98.135.188` , if your CLUSTER-IP is different, please accordingly **update** the last line of `statefulBash.yaml` :
  
  ` args: ["10.98.135.188"]` => `args: ["Your plusar cluster ip"]`
  
  Then you can run `kubectl apply -f statefulBash.yaml` and there would be two stateful bashruntime replicas running:
  
  ![image](https://user-images.githubusercontent.com/45562036/160540931-4e765540-6cde-44e9-8ff1-dbc820ff84ae.png)

  To check the correctness, do `kubectl logs stateful-bashruntime-0`. From the logs, we see bashruntime has successfully connected to Pulsar, run the bash and produced the result to another Pulsar topic!
  ![image](https://user-images.githubusercontent.com/45562036/160541179-69c83d80-298e-4633-8079-090d410660c0.png)

  
</details>

   
