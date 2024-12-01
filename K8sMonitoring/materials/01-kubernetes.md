# Kubernetes

**Kubernetes** is a powerful framework for orchestrating application containers developed by Google. Like docker swarm, it performs the basic functions of any orchestration tool: high availability, protection of internal traffic, encapsulation of a complex microservice application architecture into a single entity, and so on. But what is the main difference between **Kubernetes** and **Docker Swarm**? Besides higher complexity and more container control tools, the main feature of **Kubernetes** is the fact that it is a higher-level solution than **Docker Swarm** and is not tied directly to *Docker* technology. Yes, at the moment docker is the absolute leader in application containerization, but still it is not the only technology, and **Kubernetes** offers a unified approach regardless of the containerization tool.

## The architecture and principles of Kubernetes

Conceptually, a Kubernetes cluster is the following structure:

<img src="misc/images/kubernetes.drawio.png"  width="600">

The architecture of Kubernetes includes the following main components:

*Master node* is the main node that manages all the nodes in a Kubernetes cluster. It consists of several components:

1. kube-apiserver: a component that provides an API for managing a Kubernetes cluster.
2. etcd: a key-value store used to store Kubernetes cluster configuration data.
3. kube-scheduler: a component responsible for scheduling work on nodes in a Kubernetes cluster.
4. kube-controller-manager: is a component that manages the controllers that are responsible for performing operations such as scaling and fault recovery.

A *Node* or *Worker node* is the node on which application containers run. It consists of the following components:

1. kubelet: a component that manages containers on the node and communicates with the kube-apiserver on the master node.
2. kube-proxy: a component that is responsible for routing network requests to containers on the node.

A *Pod* is the smallest unit in Kubernetes that contains one or more application containers. *Pod* is a minimal Kubernetes abstraction due to the fact that Kubernetes as a technology seeks to move away from being bound to a specific containerization tool, hence the need to create a new type of abstraction, which is an additional "container for container". Each *Pod* has its own IP address (determined dynamically when it runs) and shares the resources of the host it is running on. It is important to understand that when a *Pod* is restarted, its IP address changes because a new *Pod* is actually being started.

But there are many other objects necessary to understand the Kubernetes architecture, most of which are listed below:

The *Service* is a **Kubernetes** object that provides a persistent IP address and DNS name for accessing the application within the cluster. It can route requests to different *Pods* based on selectors. Moreover, when replicating *Pod*, the *service* object also acts as a balancer, distributing requests between replicas.

The *Volume* is a **Kubernetes** object that is used to store data that containers in *Pod* need. Volume can be connected to an application container to provide access to data.

The *Namespace* is a **Kubernetes** object used to group cluster resources and share access to those resources between users and teams.

In **Kubernetes**, replication is a mechanism that allows you to create multiple copies of the same application and run them on multiple nodes. This ensures high availability of the application and also allows you to handle large workloads.

Replication Controller manages the process of creating and scaling replicated pods in Kubernetes. When a replication controller is created, it creates the required number of pods specified in the controller specification. If any pod fails or is deleted, the controller automatically creates a new pod to replace the lost one.

Replicated pods have a *label* that helps the replica controller manage them. Labels are also used so that services can determine which pods should receive traffic.

Replica controllers can be used to scale applications horizontally, by increasing or decreasing the number of replicated pods. This allows you to respond quickly to changes in application load.

In **Kubernetes**, *Deployment* and *StatefulSet* are objects that control application runtime and scaling. Both objects are a declarative way of defining the desired state of an application and automatically managing its life cycle.

*Deployment* provides management of application deployment (installation) in **Kubernetes**. It describes how and when to create instances of the application as well as what updates to make when the configuration changes. *Deployment* automatically creates and manages multiple replicas of the application for fault tolerance and scalability.

*StatefulSet* provides control over the installation and scaling of applications that have state. This can be useful, for example, for databases that store data on hard disks and that cannot simply be copied and run on another cluster node. *StatefulSet* provides unique names for each application instance, stores their state, and provides unique host IDs for each instance.

In both cases, **Kubernetes** automatically manages the application life cycle, scaling, updating and rolling back changes. Developers can define the desired state of the application, and **Kubernetes** will ensure that it is achieved and maintained throughout the application life cycle.

The *Ingress* in **Kubernetes** is an object that allows you to manage incoming traffic to the cluster. *Ingress* serves as a controller that defines traffic routing rules between services in the cluster and the outside world. It allows developers and administrators to manage incoming traffic, configure routing and security, and perform load balancing. In addition, *Ingress* can be used to configure SSL encryption and client authentication. Overall, using *Ingress* simplifies network management and provides a more flexible and efficient application performance on **Kubernetes**.

*ConfigMap* and *Secret* are two mechanisms in Kubernetes for storing configuration data and secrets, respectively.

*ConfigMap* is used to store configuration data that is used by applications in containers (usually as environment variables). For example, these can be parameters that need to be changed when deploying an application, such as a database address or a web server port. *ConfigMap* can be applied to any number of containers in a pod.

*Secret* is used to store sensitive information such as passwords, keys and certificates. *Secret* can be used for any number of containers in the pod. It is important to note that *Secret* is stored encrypted in the etcd of the Kubernetes cluster.

Using *ConfigMap* and *Secret* reduces the number of required parameters in application manifests and improves application security, since confidential information will not be stored in clear form.

## The kubectl command line interface

Kubectl is a Kubernetes API client, providing access to Kubernetes environment functions. The Kubernetes API is an HTTP REST API server providing access to all Kubernetes functions as endpoints for HTTP requests.

### Basic kubectl commands

* config

Configuring and displaying the system configuration (kubeconfig)

`kubectl config view`

* apply

Managing Kubernetes resources

`kubectl apply -f <relative path to the manifest>`

`kubectl explain pods`

* get

Viewing and finding system resources

`kubectl get <resource name> --<search parameters>`

* edit, scale, delete

Editing specific system resources. The most popular commands when maintaining a Kubernetes system are:

`kubectl edit <service name>`

`kubectl scale --replicas=<number of services> <service name|path to the manifest file>`

`kubectl delete <service name|path to the manifest file>`

* logs

An important tool when debugging the system in case of errors. Often the problem lies not only in the incorrect configuration of the orchestration system, but also in the software product itself, running in the environment.

To debug a software product, you can use the command

`kubectl logs <service name>`

## Manifest

A Kubernetes manifest is a file that describes the desired state of a Kubernetes object. A manifest can contain many parameters and configuration settings for an object, such as a deployment, a service, a configuration map, etc.

The general structure of the manifest in Kubernetes:

```yml
apiVersion: <API version> # version of the Kubernetes API that the object uses
kind: <Object type> # object type (deployment, service, configmap и т.д.)
metadata: # object's metadata, such as title, labels, etc.
  name: <Object name>
  labels:
    <Object labels>
spec: # object specification describing the desired state
  <Object parameters>
```

For example, the manifest for a deployment might look like this:


```yml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: my-deployment
  labels:
    app: my-app
spec:
  replicas: 3
  selector:
    matchLabels:
      app: my-app
  template:
    metadata:
      labels:
        app: my-app
    spec:
      containers:
      - name: my-container
        image: my-image
        ports:
        - containerPort: 8080
```

In this example, we specify the desired number of replicas (3), define a selector to select pods ( labeled "app: my-app") and describe the pod template that will be created if needed. In the template, we specify the name of the container, the image used and the port to be opened in the container.

The practice of splitting manifests into different files, showing a step-by-step deployment, is often used. For example, you might create a configure map first, then secrets, and then services, etc.
