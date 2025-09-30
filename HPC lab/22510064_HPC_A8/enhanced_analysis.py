import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

print("=== HPC ASSIGN 8 PERFORMANCE ANALYSIS ===\n")

def parse_csv_line(line):
    parts = line.strip().split(',')
    result = {}
    for part in parts:
        if '=' in part:
            key, value = part.split('=', 1)
            if key in ['NX', 'NY', 'procs']:
                result[key] = int(value)
            elif key in ['time_total', 'time_comp', 'time']:
                result[key] = float(value)
            elif key in ['dot']:
                result[key] = float(value)
            else:
                result[key] = value
        else:
            result['tag'] = part
    return result

conv_data = []
with open('conv_results.csv', 'r') as f:
    for line in f:
        if line.strip():
            conv_data.append(parse_csv_line(line))

df_conv = pd.DataFrame(conv_data)

print("1. CONVOLUTION ANALYSIS")
print("-" * 40)

conv_means = df_conv.groupby('procs').agg({
    'time_total': 'mean',
    'time_comp': 'mean'
}).reset_index()

conv_means['comm_time'] = conv_means['time_total'] - conv_means['time_comp']
conv_means['comm_percentage'] = (conv_means['comm_time'] / conv_means['time_total']) * 100
base_time = conv_means.loc[conv_means['procs']==1,'time_total'].values[0]
conv_means['speedup'] = base_time / conv_means['time_total']
conv_means['efficiency'] = conv_means['speedup'] / conv_means['procs']

print(f"{'Procs':<6} {'Total(s)':<10} {'Comp(s)':<10} {'Comm(s)':<10} {'Comm%':<8} {'Speedup':<8} {'Efficiency':<10}")
print("-" * 70)
for _, row in conv_means.iterrows():
    print(f"{row['procs']:<6} {row['time_total']:<10.6f} {row['time_comp']:<10.6f} "
          f"{row['comm_time']:<10.6f} {row['comm_percentage']:<8.1f} "
          f"{row['speedup']:<8.2f} {row['efficiency']*100:<10.1f}%")

# Dot Product Analysis
dot_data = []
with open('dot_results.csv', 'r') as f:
    for line in f:
        if line.strip():
            dot_data.append(parse_csv_line(line))

df_dot = pd.DataFrame(dot_data)

print("\n2. DOT PRODUCT ANALYSIS")
print("-" * 40)

dot_means = df_dot.groupby('procs')['time'].mean().reset_index()
base_time_dot = dot_means.loc[dot_means['procs']==1,'time'].values[0]
dot_means['speedup'] = base_time_dot / dot_means['time']
dot_means['efficiency'] = dot_means['speedup'] / dot_means['procs']

print(f"{'Procs':<6} {'Time(s)':<10} {'Speedup':<8} {'Efficiency':<10}")
print("-" * 40)
for _, row in dot_means.iterrows():
    print(f"{row['procs']:<6} {row['time']:<10.6f} {row['speedup']:<8.2f} {row['efficiency']*100:<10.1f}%")

# Create comprehensive plots
fig, axes = plt.subplots(2, 3, figsize=(15, 10))
fig.suptitle('HPC Performance Analysis: Convolution vs Dot Product', fontsize=16)

# Convolution plots
axes[0,0].plot(conv_means['procs'], conv_means['time_total'], 'bo-', label='Total Time')
axes[0,0].plot(conv_means['procs'], conv_means['time_comp'], 'ro-', label='Computation Time')
axes[0,0].plot(conv_means['procs'], conv_means['comm_time'], 'go-', label='Communication Time')
axes[0,0].set_xlabel('Processes')
axes[0,0].set_ylabel('Time (s)')
axes[0,0].set_title('Convolution: Time Breakdown')
axes[0,0].legend()
axes[0,0].grid(True)

axes[0,1].plot(conv_means['procs'], conv_means['speedup'], 'bo-', label='Actual')
axes[0,1].plot(conv_means['procs'], conv_means['procs'], 'r--', label='Ideal')
axes[0,1].set_xlabel('Processes')
axes[0,1].set_ylabel('Speedup')
axes[0,1].set_title('Convolution: Speedup')
axes[0,1].legend()
axes[0,1].grid(True)

axes[0,2].plot(conv_means['procs'], conv_means['efficiency']*100, 'bo-')
axes[0,2].set_xlabel('Processes')
axes[0,2].set_ylabel('Efficiency (%)')
axes[0,2].set_title('Convolution: Efficiency')
axes[0,2].set_ylim(0, 100)
axes[0,2].grid(True)

# Dot product plots
axes[1,0].plot(dot_means['procs'], dot_means['time'], 'mo-')
axes[1,0].set_xlabel('Processes')
axes[1,0].set_ylabel('Time (s)')
axes[1,0].set_title('Dot Product: Execution Time')
axes[1,0].grid(True)

axes[1,1].plot(dot_means['procs'], dot_means['speedup'], 'mo-', label='Actual')
axes[1,1].plot(dot_means['procs'], dot_means['procs'], 'r--', label='Ideal')
axes[1,1].set_xlabel('Processes')
axes[1,1].set_ylabel('Speedup')
axes[1,1].set_title('Dot Product: Speedup')
axes[1,1].legend()
axes[1,1].grid(True)

axes[1,2].plot(dot_means['procs'], dot_means['efficiency']*100, 'mo-')
axes[1,2].set_xlabel('Processes')
axes[1,2].set_ylabel('Efficiency (%)')
axes[1,2].set_title('Dot Product: Efficiency')
axes[1,2].set_ylim(0, 100)
axes[1,2].grid(True)

plt.tight_layout()
plt.savefig('enhanced_hpc_analysis.png', dpi=300, bbox_inches='tight')
plt.show()

# Comparison analysis
print("\n3. ALGORITHM COMPARISON")
print("-" * 40)
print("Communication Overhead Analysis (Convolution):")
for _, row in conv_means.iterrows():
    print(f"  {row['procs']} processes: {row['comm_percentage']:.1f}% communication overhead")

print(f"\nScalability Comparison (4 processes):")
conv_4p_eff = conv_means.loc[conv_means['procs']==4,'efficiency'].values[0] * 100
dot_4p_eff = dot_means.loc[dot_means['procs']==4,'efficiency'].values[0] * 100
print(f"  Convolution efficiency: {conv_4p_eff:.1f}%")
print(f"  Dot Product efficiency: {dot_4p_eff:.1f}%")

# Calculate theoretical FLOPS (rough estimate)
conv_ops_per_pixel = 9  # 3x3 kernel
total_pixels = 1024 * 1024
conv_flops = conv_ops_per_pixel * total_pixels
dot_flops = 10000000 * 2  # multiply + add per element

print(f"\nComputational Intensity:")
print(f"  Convolution: ~{conv_flops/1e6:.1f} MFLOPS")
print(f"  Dot Product: ~{dot_flops/1e6:.1f} MFLOPS")

conv_1p_gflops = conv_flops / (conv_means.loc[conv_means['procs']==1,'time_comp'].values[0] * 1e9)
dot_1p_gflops = dot_flops / (dot_means.loc[dot_means['procs']==1,'time'].values[0] * 1e9)

print(f"\nPeak Performance (1 process):")
print(f"  Convolution: {conv_1p_gflops:.2f} GFLOPS")
print(f"  Dot Product: {dot_1p_gflops:.2f} GFLOPS")