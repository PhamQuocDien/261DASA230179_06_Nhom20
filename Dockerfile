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
       -o /app/bin/library_api.exe \
    && chmod +x /app/bin/library_api.exe

RUN chown -R www-data:www-data /app/data /app/bin

RUN sed -ri \
    -e 's!/var/www/html!/app/src!g' \
    /etc/apache2/sites-available/000-default.conf \
    /etc/apache2/apache2.conf

RUN sed -ri \
    -e 's/Listen 80/Listen 10000/g' \
    /etc/apache2/ports.conf \
    /etc/apache2/sites-available/000-default.conf

RUN sed -ri \
    -e 's/<VirtualHost \*:80>/<VirtualHost *:10000>/g' \
    /etc/apache2/sites-available/000-default.conf

EXPOSE 10000

CMD ["apache2-foreground"]
