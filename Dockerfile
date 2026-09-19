FROM php:8.3-apache

RUN apt-get update \
    && apt-get install -y g++ nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN mkdir -p /app/bin \
    && g++ -std=c++17 \
       -I/app/src \
       /app/src/main.cpp \
       $(find /app/src/dsa_core /app/src/persistence -name "*.cpp") \
       -o /app/bin/ThuVien.exe \
    && chmod +x /app/bin/ThuVien.exe

RUN chown -R www-data:www-data /app/data /app/bin

# Apache dùng src làm thư mục web
RUN sed -ri \
    -e 's!/var/www/html!/app/src!g' \
    /etc/apache2/sites-available/000-default.conf

# Render dùng port 10000
RUN sed -ri \
    -e 's/Listen 80/Listen 10000/g' \
    /etc/apache2/ports.conf \
    /etc/apache2/sites-available/000-default.conf

RUN sed -ri \
    -e 's/<VirtualHost \*:80>/<VirtualHost *:10000>/g' \
    /etc/apache2/sites-available/000-default.conf

# Cho Apache quyền truy cập /app/src
RUN printf '%s\n' \
    '<Directory /app/src>' \
    '    Options Indexes FollowSymLinks' \
    '    AllowOverride All' \
    '    Require all granted' \
    '</Directory>' \
    > /etc/apache2/conf-available/project.conf \
    && a2enconf project

# Loại cảnh báo ServerName
RUN printf '%s\n' 'ServerName localhost' \
    > /etc/apache2/conf-available/servername.conf \
    && a2enconf servername

EXPOSE 10000

CMD ["apache2-foreground"]
