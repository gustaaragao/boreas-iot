```sh
docker ps
```

```sh
docker-compose up --build -d
```

```sh
docker exec -it <CONTAINER_ID>  sh
```

### Configurações do `mosquitto.conf`

```sh
# Listen on port 1883 
listener 1883
listener 9001

# Save the in-memory database to disk
persistence true
persistence_location /mosquitto/data/

# Log to stderr and logfile
log_dest stderr
log_dest file /mosquitto/log/mosquitto.log
```

https://www.youtube.com/watch?v=rid5M4ifGPA&t=235s

https://docs.arduino.cc/tutorials/portenta-x8/datalogging-iot/

https://www.instructables.com/A-Simple-MQTT-PubSub-Node-With-Arduino-UNO-and-W51/

https://www.youtube.com/watch?v=_DO2wHI6JWQ&t=49s