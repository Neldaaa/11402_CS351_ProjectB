FROM ubuntu:22.04

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Configure and build the project
RUN cmake -S . -B build && cmake --build build

# By default, run the tests when the container starts
CMD ["cd", "build", "&&", "ctest", "--output-on-failure"]