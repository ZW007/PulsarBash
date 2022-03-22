# FROM apachepulsar/pulsar-build-cpp:ubuntu-16.04
FROM apachepulsar/pulsar:latest
COPY lib/libpulsar* /usr/lib/
COPY lib/libstdc* /usr/lib/x86_64-linux-gnu/ 
COPY ./* /pulsar_bash/

WORKDIR /pulsar_bash
# RUN chmod 777 ./bashruntime.sh
# ENTRYPOINT ./bashruntime.sh
ENTRYPOINT ./bashruntime