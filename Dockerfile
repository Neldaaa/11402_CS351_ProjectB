# Sử dụng image Ubuntu mới nhất làm base
FROM ubuntu:latest

# Cài đặt các công cụ cần thiết để build C++ và CMake
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# Thiết lập thư mục làm việc trong container
WORKDIR /app

# Copy toàn bộ mã nguồn vào container
COPY . .

# Build project bằng CMake
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build --parallel

# Lệnh mặc định khi chạy container (chạy project chính csv_db)
CMD ["./build/csv_db"]