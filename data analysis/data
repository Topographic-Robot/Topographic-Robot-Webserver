import dash.dash
import dash
from dash import dcc, html
from dash.dependencies import Input,Output
import pandas as pd
import plotly.express as px
import json

# json files
def load_files(file_path,key):
    timestamps= []
    sensordata=[]
    with open(file_path, 'r') as f:
        for line in f:
            m=line.strip()
           
            if line:
                k=m.split(" ",2)
                timestamp1=k[1]
                json_data=json.loads(k[2])
                timestamps.append(timestamp1)
                sensordata.append(json_data)
        df=pd.DataFrame(sensordata)
        df["timestamp"] = pd.to_datetime(timestamps) 
    return df
#loading the data
temp_sensor= load_files("c:\\Users\\roshan06\\Downloads\\DHT22.TXT",'temperature_c')
gyro_sensor= load_files("c:\\Users\\roshan06\\Downloads\\MPU6050 (1).TXT",'x')

#dash dashboard
app= dash.Dash(__name__)
app.layout= html.Div([
    html.H1("analyzed data"),
    dcc.Interval(id='update-interval',interval=5000,n_intervals=0),
    dcc.Graph(id='temp-graph'),
    dcc.Graph(id='gyro-graph'),
])

#callback function for dynamical update
@app.callback(
        [
            Output('temp-graph','figure'),
            Output('gyro-graph','figure')

        ],
        [Input('update-interval', 'n_intervals')]
    )
def update_graphs(n):
    temp_fig = px.line(temp_sensor, y="timestamp", x="temperature_c", title="Temperature Over Time")
    gyro_fig = px.line(gyro_sensor, x="timestamp", y="gyro_x", title="Gyro X-Axis Over Time")
    return temp_fig, gyro_fig
# Run app
if __name__ == '__main__':
    app.run_server(debug=True)
