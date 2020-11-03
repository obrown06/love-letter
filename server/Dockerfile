FROM gcc:9.3.0

RUN apt-get -qq update
RUN apt-get -qq upgrade

# Install tools required for the project
RUN apt-get update \
    && apt-get install gcc -y \
    && apt-get install g++ -y \
    && apt-get install cmake -y \
    && apt-get install wget -y

# Install Boost
RUN cd /home \
    && wget http://downloads.sourceforge.net/project/boost/boost/1.70.0/boost_1_70_0.tar.gz \
    && tar xfz boost_1_70_0.tar.gz \
    && rm boost_1_70_0.tar.gz \
    && cd boost_1_70_0 \
    && ./bootstrap.sh --with-libraries=system \
    && ./b2 install

# Install JSONCPP
RUN apt-get install -y libjsoncpp-dev

COPY . /cpp/src/project/
WORKDIR /cpp/src/project/

RUN make

ENTRYPOINT ["build/a.out", "0.0.0.0", "$PORT"]
