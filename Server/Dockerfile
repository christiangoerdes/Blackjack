FROM ubuntu:22.04

WORKDIR /app

# Install dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    libboost-all-dev \
    python3-dev \
    curl \
    ca-certificates \
    python3-pip

# Install pybind11
RUN git clone https://github.com/pybind/pybind11.git && \
    cd pybind11 && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make install

# Download and install a specific version of CMake (3.21.2 in this example)
RUN curl -sSL https://github.com/Kitware/CMake/releases/download/v3.21.2/cmake-3.21.2-linux-x86_64.tar.gz -o cmake.tar.gz && \
    tar -xf cmake.tar.gz --strip-components=1 -C /usr/local && \
    rm cmake.tar.gz

# Install Python dependencies first to leverage Docker build cache
RUN pip install --no-cache-dir \
    fastapi \
    uvicorn \
    pytest

# Copy the API directory
COPY ./api /app/api

# Copy the include directory
COPY ./include /app/include

# Copy the src directory
COPY ./src /app/src

# Copy the test directory
COPY ./test /app/test

# Copy the cmake
COPY ./CMakeLists.txt /app

# Build the C++ code with Pybind11
RUN cmake -S . -B build && cmake --build build && cmake --install build

# Expose the required port
EXPOSE 8000

# Set the working directory to the api directory
WORKDIR /app/api

# Run the API
CMD ["uvicorn", "api:api", "--port", "8000", "--reload"]
