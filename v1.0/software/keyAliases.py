import pandas as pd

df = pd.read_excel('keyAliases.xlsx', header=None)
rows, cols = df.shape

aliases = []
values = []

for i in range (4,rows,7):
    aliases.append(df[0][i-1])
    values_s = []
    for j in range (0,6):
        for k in range (0,18):
            value = df[k+1][i-1+j]
            if not pd.isna(value): values_s.append(value)
    values.append(values_s)

data = dict(zip(aliases,values))

with open(".\RCK\keyAliases.h","w") as f:
    f.write("// Key aliases\n\n")
    f.write("#define XXXXXX\t\tKEY_ERROR_UNDEFINED\n\n")
    for aliase in data:
        if aliase == 'DEFAULT': continue
        for i,key in enumerate(data[aliase]):
            f.write("#define {}_{}\t\t{}\n".format(aliase,data['DEFAULT'][i],key))