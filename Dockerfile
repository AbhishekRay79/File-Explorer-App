FROM ubuntu:22.04
RUN apt update && apt install -y g++ build-essential nano
WORKDIR /app
COPY . .
CMD ["/bin/bash"]
