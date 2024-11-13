# Topographic-Robot-Webserver
MongoDB webserver that will receive the data sent by the ESP32.

### Test

```
curl -X POST -d '{"sensor_type": "temperature", "value": 25.3, "timestamp": 1636118452}' http://localhost:8888
```

```
bsikar@Brightons-MacBook-Air Topographic-Robot-Webserver % mongosh

Current Mongosh Log ID:	6734b955ce9c15a1bf2a17fb
Connecting to:		mongodb://127.0.0.1:27017/?directConnection=true&serverSelectionTimeoutMS=2000&appName=mongosh+2.3.3
(node:11942) ExperimentalWarning: CommonJS module /opt/homebrew/Cellar/mongosh/2.3.3/libexec/lib/node_modules/@mongosh/cli-repl/node_modules/@mongodb-js/devtools-proxy-support/dist/fetch.js is loading ES Module /opt/homebrew/Cellar/mongosh/2.3.3/libexec/lib/node_modules/@mongosh/cli-repl/node_modules/node-fetch/src/index.js using require().
Support for loading ES Module in require() is an experimental feature and might change at any time
(Use `node --trace-warnings ...` to show where the warning was created)
Using MongoDB:		6.0.18
Using Mongosh:		2.3.3

For mongosh info see: https://www.mongodb.com/docs/mongodb-shell/


To help improve our products, anonymous usage data is collected and sent to MongoDB periodically (https://www.mongodb.com/legal/privacy-policy).
You can opt-out by running the disableTelemetry() command.

------
   The server generated these startup warnings when booting
   2024-11-13T08:20:17.704-06:00: Access control is not enabled for the database. Read and write access to data and configuration is unrestricted
------

test> show dbs
admin      40.00 KiB
config     12.00 KiB
local      40.00 KiB
sensor_db  40.00 KiB
test> use sensor_db
switched to db sensor_db
sensor_db> show collections
sensor_data
sensor_db> db.sensor_data.find()
[
  {
    _id: ObjectId('6734b81c8d283bfa1b073071'),
    sensor_type: 'temperature',
    value: 25.3,
    timestamp: 1636118452
  },
  {
    _id: ObjectId('6734b8208d283bfa1b073072'),
    sensor_type: 'temperature',
    value: 25.3,
    timestamp: 1636118452
  },
  {
    _id: ObjectId('6734b8218d283bfa1b073073'),
    sensor_type: 'temperature',
    value: 25.3,
    timestamp: 1636118452
  },
  {
    _id: ObjectId('6734b8218d283bfa1b073074'),
    sensor_type: 'temperature',
    value: 25.3,
    timestamp: 1636118452
  },
  {
    _id: ObjectId('6734b8228d283bfa1b073075'),
    sensor_type: 'temperature',
    value: 25.3,
    timestamp: 1636118452
  },
  {
    _id: ObjectId('6734b8228d283bfa1b073076'),
    sensor_type: 'temperature',
    value: 25.3,
    timestamp: 1636118452
  },
  {
    _id: ObjectId('6734b8238d283bfa1b073077'),
    sensor_type: 'temperature',
    value: 25.3,
    timestamp: 1636118452
  }
]
```

### Getting the data once added:

```
mongoexport --db=sensor_db --collection=sensor_data --out=sensor_data.json --jsonArray
```

or using a program, in utils there is c code to do this
