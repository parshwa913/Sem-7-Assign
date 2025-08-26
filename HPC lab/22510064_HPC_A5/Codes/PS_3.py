import pandas as pd
import matplotlib.pyplot as plt

# Load CSV file
df = pd.read_csv("PS_3.csv")

# Plot 1: Execution Time vs Matrix Size (for a fixed thread count, e.g., 4 threads)
plt.figure()
for t in df["Threads"].unique():
    subset = df[df["Threads"] == t]
    plt.plot(subset["DataSize"], subset["Time(s)"],
             marker="o", label=f"{t} Threads")
plt.xlabel("Matrix Size (N x N)")
plt.ylabel("Execution Time (s)")
plt.title("Execution Time vs Matrix Size")
plt.legend()
plt.grid(True)
plt.savefig("PS_3_Execution_Time_vs_Size.png")
plt.show()

# Plot 2: Execution Time vs Number of Threads (for a fixed matrix size)
plt.figure()
for size in df["DataSize"].unique():
    subset = df[df["DataSize"] == size]
    plt.plot(subset["Threads"], subset["Time(s)"],
             marker="o", label=f"Size {size}x{size}")
plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (s)")
plt.title("Execution Time vs Number of Threads")
plt.legend()
plt.grid(True)
plt.savefig("PS_3_Execution_Time_vs_Threads.png")
plt.show()

# Plot 3: Speedup vs Number of Threads
plt.figure()
for size in df["DataSize"].unique():
    subset = df[df["DataSize"] == size].sort_values("Threads")
    baseline = subset[subset["Threads"] ==
                      1]["Time(s)"].values[0]  # sequential time
    speedup = baseline / subset["Time(s)"]
    plt.plot(subset["Threads"], speedup, marker="o",
             label=f"Size {size}x{size}")
plt.xlabel("Number of Threads")
plt.ylabel("Speedup (T1 / Tn)")
plt.title("Speedup vs Number of Threads")
plt.legend()
plt.grid(True)
plt.savefig("PS_3_Speedup_vs_Threads.png")
plt.show()
