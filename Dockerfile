FROM gcc:12

RUN apt-get update -y && apt-get install -y libssl-dev

WORKDIR /usr/src/myapp

