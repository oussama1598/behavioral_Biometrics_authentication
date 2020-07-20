# get and configure an image
FROM debian:stretch-slim
RUN apt-get update
RUN apt-get install -y --no-install-recommends \
    ca-cacert \
    cmake \
    build-essential \
    libboost-all-dev \
    libssl-dev \
    wget \
    zlib1g-dev \
    python

# get and build wt4
WORKDIR /root
RUN pip install conan

# build our app
WORKDIR /root/behavioral_biemetrics_authentication
ADD . /root/behavioral_biemetrics_authentication
RUN mkdir cmake-build-release && cd cmake-build-release
RUN cmake -DCMAKE_BUILD_TYPE=Release ..
RUN make -j2
CMD ./api_server_api
