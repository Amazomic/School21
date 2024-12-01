# Deployment strategies

Kubernetes provides several deployment strategies that determine how an application is updated in a Kubernetes cluster.

1. Recreate is the simplest strategy. When you update an application, all current pods are deleted first, and then new pods are created with the new version of the application. This strategy can lead to some temporary downtime of the application, when all current pods are deleted and no new pods are created yet.

2. Rolling Update is the most popular strategy. When an application is updated, new pods gradually run and replace the old ones. The number of pods of the old and new version of the application may vary until all pods have been replaced by the new version.

3. Blue/Green Deployment - this strategy creates two separate groups of pods, called Blue and Green. Initially, traffic is directed to the Blue group, and a new version of the application is deployed in the Green group. When the Green group is fully ready, traffic is switched to it. If a failure occurs during the switch, you can quickly return to the previous version of the application.

4. Canary Deployment - with this strategy, the new version of the application is deployed only to a small group of users to test its functionality and performance. If all works well, the new version is gradually deployed to all users.

5. A/B Testing - with this strategy, different versions of the application are deployed to different groups of users to test which version works better. Typically used to test new features or changes to the user interface.

Typically, Kubernetes manifests for each deployment strategy look different, but in general, they all have a similar structure and describe the resources required to run and update the application.
