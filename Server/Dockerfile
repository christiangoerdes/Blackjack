# Stage 1: Build stage with C++ and Pybind11
FROM gcc as builder

WORKDIR /app

# Update and upgrade the system
RUN apt-get update && apt-get upgrade -y

# Install dependencies
RUN apt-get install -y cmake python3-dev libboost-all-dev python3-pip --no-install-recommends

# Copy the API directory
COPY ./api /app/api

# Copy the external directory
COPY ./external /app/external

# Copy the include directory
COPY ./include /app/include

# Copy the src directory
COPY ./src /app/src

# Copy the test directory
COPY ./test /app/test

# Copy the cmake
COPY ./CMakeLists.txt /app

# Install pybind11
RUN git clone https://github.com/pybind/pybind11.git && \
    cd pybind11 && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make install

# Build the C++ code with Pybind11
RUN cmake -S . -B build && cmake --build build && cmake --install build

# Stage 2: Final stage with Python dependencies and runtime
FROM python:3.9-slim

WORKDIR /app

# Stage 2: Runtime stage with Python and Pybind11
FROM python:3.9-slim
WORKDIR /app

# Copy the built C++ code from the previous stage
COPY --from=builder /app /app

# Install Python dependencies
RUN pip install --no-cache-dir \
    fastapi \
    uvicorn \
    pytest

# Copy the API directory
COPY ./api /app/api

# Expose the required port
EXPOSE 8000

# Set the working directory to the api directory
WORKDIR /app/api

# Run the API
CMD ["uvicorn", "api:api", "--port", "8000", "--reload"]
