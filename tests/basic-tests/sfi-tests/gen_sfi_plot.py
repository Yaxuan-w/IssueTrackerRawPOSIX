import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import json
import sys


dataset = dict()
dataset["relative_time"] = list()
dataset["platform"] = list()
dataset["time"] = list()
dataset["error"] = list()

nativeavgs = dict()

with open(sys.argv[1], "r") as fp:
    native_close = json.load(fp)
with open(sys.argv[2], "r") as fp:
    lind_close = json.load(fp)

for key in lind_close:
    lind_avg = np.mean(lind_close[key])
    native_avg = np.mean(native_close[key])

    dataset["relative_time"].append(lind_avg / native_avg)
    dataset["time"].append(lind_avg)
    dataset["platform"].append("Lind close()")
    dataset["error"].append(
        np.std(
            [
                lind_close[key][i] / np.mean(native_close[key])
                for i in range(len(lind_close[key]))
            ]
        )
    ) 

for key in native_close:
    native_avg = np.mean(native_close[key])
    native_se = np.std(native_close[key]) / np.sqrt(len(native_close[key]))  
    
    dataset["relative_time"].append(1)  
    dataset["time"].append(native_avg)
    dataset["platform"].append("Native close()")
    dataset["error"].append(
        np.std(
            [
                native_close[key][i] / np.mean(native_close[key])
                for i in range(len(native_close[key]))
            ]
        )
    )

with open(sys.argv[3], "r") as fp:
    native_getpid = json.load(fp)
with open(sys.argv[4], "r") as fp:
    lind_getpid = json.load(fp)

for key in lind_getpid:
    lind_avg = np.mean(lind_getpid[key])
    native_avg = np.mean(native_getpid[key])

    dataset["relative_time"].append(lind_avg / native_avg)
    dataset["time"].append(lind_avg)
    dataset["platform"].append("Lind getpid()")
    dataset["error"].append(
        np.std(
            [
                lind_getpid[key][i] / np.mean(native_getpid[key])
                for i in range(len(lind_getpid[key]))
            ]
        )
    ) 

for key in native_getpid:
    native_avg = np.mean(native_getpid[key])
    native_se = np.std(native_getpid[key]) / np.sqrt(len(native_getpid[key]))  
    
    dataset["relative_time"].append(1)  
    dataset["time"].append(native_avg)
    dataset["platform"].append("Native getpid()")
    dataset["error"].append(
        np.std(
            [
                native_getpid[key][i] / np.mean(native_getpid[key])
                for i in range(len(native_getpid[key]))
            ]
        )
    )

with open(sys.argv[5], "r") as fp:
    native_write = json.load(fp)
with open(sys.argv[6], "r") as fp:
    lind_write = json.load(fp)

for key in lind_write:
    lind_avg = np.mean(lind_write[key])
    native_avg = np.mean(native_write[key])

    dataset["relative_time"].append(lind_avg / native_avg)
    dataset["time"].append(lind_avg)
    dataset["platform"].append("Lind write()")
    dataset["error"].append(
        np.std(
            [
                lind_write[key][i] / np.mean(native_write[key])
                for i in range(len(lind_write[key]))
            ]
        )
    ) 

for key in native_write:
    native_avg = np.mean(native_write[key])
    native_se = np.std(native_write[key]) / np.sqrt(len(native_write[key]))  
    
    dataset["relative_time"].append(1)  
    dataset["time"].append(native_avg)
    dataset["platform"].append("Native write()")
    dataset["error"].append(
        np.std(
            [
                native_write[key][i] / np.mean(native_write[key])
                for i in range(len(native_write[key]))
            ]
        )
    )

plt.rcParams["errorbar.capsize"] = 10
df = pd.DataFrame(data=dataset)
df["ymin"] = df["relative_time"] - df["error"]
df["ymax"] = df["relative_time"] + df["error"]
yerr = df[["ymin", "ymax"]].T.to_numpy()

pd.set_option("display.max_rows", None, "display.max_columns", None)
plt.figure(figsize=(10, 6))
sns.set(style="darkgrid")
# sns.set_palette("Blues_r")
fig = sns.barplot(y="relative_time", hue="platform", data=df, palette=sns.color_palette("Paired", 6))
sns.move_legend(
    fig,
    "lower center",
    bbox_to_anchor=(0.5, -0.2),
    ncol=3,
    title="",
    fontsize=10,
    frameon=False,
)

for i, p in enumerate(fig.patches):
    if i >= len(df["ymin"]):
        break
    x = p.get_x()  # get the bottom left x corner of the bar
    w = p.get_width()  # get width of bar
    h = p.get_height()  # get height of bar
    min_y = df["ymin"][i]  # use h to get min from dict z
    max_y = df["ymax"][i]  # use h to get max from dict z
    plt.vlines(x + w / 2, min_y, max_y, color="k")

# plt.xlabel("$write$", fontsize=10)
plt.ylabel("Average Runtime", fontsize=10)
plt.tight_layout(pad=2)
plt.title(f"{sys.argv[7]}")
plt.savefig(sys.argv[7], dpi=400)
