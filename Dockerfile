FROM debian:latest
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y --no-install-recommends \
    gcc g++ \
    build-essential \
    libboost-all-dev \
    libssl-dev \
    wget \
    zlib1g-dev \
    python python3-pip python3-setuptools python-numpy

RUN gcc -v

# get and build wt4
WORKDIR /root
RUN pip3 install conan

RUN mkdir "$HOME/usr"
RUN export PATH="$HOME/usr/bin:$PATH"
RUN wget "https://cmake.org/files/v3.16/cmake-3.16.0-Linux-x86_64.sh"
RUN chmod +x cmake-3.16.0-Linux-x86_64.sh
RUN ./cmake-3.16.0-Linux-x86_64.sh --prefix="$HOME/usr" --exclude-subdir --skip-license

# build our app
WORKDIR /root/behavioral_biemetrics_authentication
ADD . /root/behavioral_biemetrics_authentication
RUN mkdir cmake-build-release
WORKDIR cmake-build-release
RUN /root/usr/bin/cmake -DCMAKE_BUILD_TYPE=Release ..
RUN make -j2
CMD ./bin/api_server_bin
