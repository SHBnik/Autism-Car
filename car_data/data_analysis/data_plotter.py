import pandas as pd

link = ['https://raw.githubusercontent.com/SHBnik/Autism-Car/master/car_data/mahdiar_04_13_AM_January_07_2020.csv',
       'https://raw.githubusercontent.com/SHBnik/Autism-Car/master/car_data/noshad_04_35_AM_January_07_2020.csv',
       'https://raw.githubusercontent.com/SHBnik/Autism-Car/master/car_data/taha%20amini2_03_34_AM_January_07_2020.csv',
       'https://raw.githubusercontent.com/SHBnik/Autism-Car/master/car_data/taha%20amini3_03_36_AM_January_07_2020.csv',
       'https://raw.githubusercontent.com/SHBnik/Autism-Car/master/car_data/taha%20amini_03_32_AM_January_07_2020.csv',
       'https://raw.githubusercontent.com/SHBnik/Autism-Car/master/car_data/yazdan%20_03_46_AM_January_07_2020.csv']


df = pd.read_csv(link[2])
id = df.to_csv()[2:df.to_csv().find('ACx')-1]
df = pd.read_csv(url, skiprows = 1)
time = df.values[df.index[-1]][0]

df = df.drop(df.index[-1])
df = df.astype(int)

plot_Acc = df.plot(x='time',y = ['ACx','ACy','ACz'],grid = True,figsize = (12,9),title = id)
plot_enc = df.plot(x = 'time',y = ['encoder1','encoder2'],grid = True,figsize = (12,9),title = id)

fig_Acc = plot_Acc.get_figure()
fig_Acc.savefig(id+'-Acc-'+".png")
fig_enc = plot_enc.get_figure()
fig_enc.savefig(id+'-enc-'+".png")
