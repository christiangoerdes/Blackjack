FROM python:3.9-slim-buster

WORKDIR /app

# Install dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    libboost-all-dev \
    python3-dev \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Install pybind11
RUN git clone https://github.com/pybind/pybind11.git && \
    cd pybind11 && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make install

# Download and install a specific version of CMake (3.21.2 in this example)
RUN curl -sSL https://cmake.org/files/v3.21/cmake-3.21.2-Linux-x86_64.tar.gz | tar -xz --strip-components=1 -C /usr/local

# Copy the API directory
COPY ./api /app/api

RUN rm /app/api/blackjack.cpython-310-x86_64-linux-gnu.so

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

# Install Python dependencies
RUN pip install --no-cache-dir \
    fastapi \
    uvicorn \
    pytest

# Expose the required port
EXPOSE 8000

# Set the working directory to the api directory
WORKDIR /app/api

# Run the API
CMD ["uvicorn", "api:api", "--port", "8000", "--reload"]
