import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import re

print("=== HPC ASSIGN 7 PERFORMANCE ANALYSIS ===\n")

def parse_csv_data(filename, algorithm_name):
    """Parse CSV data from results files"""
    data = []
    
    with open(filename, 'r') as f:
        content = f.read()
    
    # Extract CSV_OUTPUT lines
    csv_lines = re.findall(r'CSV_OUTPUT,(\d+),(\d+),([\d.]+)', content)
    
    for line in csv_lines:
        n, procs, time = line
        data.append({
            'algorithm': algorithm_name,
            'n': int(n),
            'procs': int(procs),
            'time': float(time)
        })
    
    # Also parse any explicit format lines like "matvec,n=154,procs=2,time=0.000049"
    explicit_lines = re.findall(r'(\w+),n=(\d+),procs=(\d+),time=([\d.]+)', content)
    
    for line in explicit_lines:
        alg, n, procs, time = line
        data.append({
            'algorithm': alg,
            'n': int(n),
            'procs': int(procs),
            'time': float(time)
        })
    
    return data

# Load data from both files
print("Loading performance data...")
matvec_data = parse_csv_data('results_matvec.csv', 'matvec')  # Update filename if needed
matmat_data = parse_csv_data('results_matmat.csv', 'matmat')

# Combine all data
all_data = matvec_data + matmat_data
df = pd.DataFrame(all_data)

print(f"Loaded {len(df)} performance measurements\n")

# Analysis for Matrix-Vector Multiplication
print("1. MATRIX-VECTOR MULTIPLICATION ANALYSIS")
print("-" * 50)

if len(matvec_data) > 0:
    df_matvec = df[df['algorithm'] == 'matvec'].copy()
    
    # Group by matrix size and processes
    matvec_summary = df_matvec.groupby(['n', 'procs'])['time'].agg(['mean', 'std', 'count']).reset_index()
    
    print(f"{'Size (n)':<8} {'Procs':<6} {'Time(s)':<12} {'Std Dev':<10} {'Samples':<8}")
    print("-" * 50)
    for _, row in matvec_summary.iterrows():
        std_str = f"{row['std']:.6f}" if not pd.isna(row['std']) else "N/A"
        print(f"{row['n']:<8} {row['procs']:<6} {row['mean']:<12.6f} {std_str:<10} {row['count']:<8}")
    
    # Calculate speedup for same matrix size
    for n in df_matvec['n'].unique():
        subset = df_matvec[df_matvec['n'] == n]
        if len(subset) > 1:
            print(f"\nSpeedup analysis for n={n}:")
            baseline = subset[subset['procs'] == subset['procs'].min()]['time'].mean()
            for procs in sorted(subset['procs'].unique()):
                current_time = subset[subset['procs'] == procs]['time'].mean()
                speedup = baseline / current_time
                efficiency = speedup / procs * 100
                print(f"  {procs} processes: {speedup:.2f}x speedup, {efficiency:.1f}% efficiency")

else:
    print("No matrix-vector data found")

# Analysis for Matrix-Matrix Multiplication
print("\n2. MATRIX-MATRIX MULTIPLICATION ANALYSIS")
print("-" * 50)

if len(matmat_data) > 0:
    df_matmat = df[df['algorithm'] == 'matmat'].copy()
    
    # Group by matrix size and processes
    matmat_summary = df_matmat.groupby(['n', 'procs'])['time'].agg(['mean', 'std', 'count']).reset_index()
    
    print(f"{'Size (n)':<8} {'Procs':<6} {'Time(s)':<12} {'Std Dev':<10} {'Samples':<8}")
    print("-" * 50)
    for _, row in matmat_summary.iterrows():
        std_str = f"{row['std']:.6f}" if not pd.isna(row['std']) else "N/A"
        print(f"{row['n']:<8} {row['procs']:<6} {row['mean']:<12.6f} {std_str:<10} {row['count']:<8}")
    
    # Calculate speedup for same matrix size
    for n in df_matmat['n'].unique():
        subset = df_matmat[df_matmat['n'] == n]
        if len(subset) > 1:
            print(f"\nSpeedup analysis for n={n}:")
            baseline = subset[subset['procs'] == subset['procs'].min()]['time'].mean()
            for procs in sorted(subset['procs'].unique()):
                current_time = subset[subset['procs'] == procs]['time'].mean()
                speedup = baseline / current_time
                efficiency = speedup / procs * 100
                print(f"  {procs} processes: {speedup:.2f}x speedup, {efficiency:.1f}% efficiency")

else:
    print("No matrix-matrix data found")

# Create visualization plots
fig, axes = plt.subplots(2, 2, figsize=(15, 10))
fig.suptitle('HPC Assignment 7: Matrix Operations Performance Analysis', fontsize=16)

# Plot 1: Execution time vs matrix size for matvec
if len(matvec_data) > 0:
    matvec_plot_data = df_matvec.groupby(['n', 'procs'])['time'].mean().reset_index()
    for procs in sorted(matvec_plot_data['procs'].unique()):
        subset = matvec_plot_data[matvec_plot_data['procs'] == procs]
        axes[0,0].plot(subset['n'], subset['time'], 'o-', label=f'{procs} processes')
    
    axes[0,0].set_xlabel('Matrix Size (n)')
    axes[0,0].set_ylabel('Execution Time (s)')
    axes[0,0].set_title('Matrix-Vector Multiplication: Time vs Size')
    axes[0,0].legend()
    axes[0,0].grid(True)
    axes[0,0].set_yscale('log')

# Plot 2: Execution time vs matrix size for matmat
if len(matmat_data) > 0:
    matmat_plot_data = df_matmat.groupby(['n', 'procs'])['time'].mean().reset_index()
    for procs in sorted(matmat_plot_data['procs'].unique()):
        subset = matmat_plot_data[matmat_plot_data['procs'] == procs]
        axes[0,1].plot(subset['n'], subset['time'], 's-', label=f'{procs} processes')
    
    axes[0,1].set_xlabel('Matrix Size (n)')
    axes[0,1].set_ylabel('Execution Time (s)')
    axes[0,1].set_title('Matrix-Matrix Multiplication: Time vs Size')
    axes[0,1].legend()
    axes[0,1].grid(True)
    axes[0,1].set_yscale('log')

# Plot 3: Speedup comparison
speedup_data = []
for algorithm in ['matvec', 'matmat']:
    subset = df[df['algorithm'] == algorithm]
    for n in subset['n'].unique():
        size_data = subset[subset['n'] == n]
        if len(size_data) > 1:
            baseline = size_data[size_data['procs'] == size_data['procs'].min()]['time'].mean()
            for procs in sorted(size_data['procs'].unique()):
                current_time = size_data[size_data['procs'] == procs]['time'].mean()
                speedup = baseline / current_time
                speedup_data.append({
                    'algorithm': algorithm,
                    'n': n,
                    'procs': procs,
                    'speedup': speedup
                })

if speedup_data:
    speedup_df = pd.DataFrame(speedup_data)
    for algorithm in speedup_df['algorithm'].unique():
        alg_data = speedup_df[speedup_df['algorithm'] == algorithm]
        # Plot for largest matrix size
        largest_n = alg_data['n'].max()
        plot_data = alg_data[alg_data['n'] == largest_n]
        marker = 'o' if algorithm == 'matvec' else 's'
        axes[1,0].plot(plot_data['procs'], plot_data['speedup'], f'{marker}-', 
                      label=f'{algorithm} (n={largest_n})')
    
    # Add ideal speedup line
    max_procs = speedup_df['procs'].max()
    axes[1,0].plot(range(1, max_procs+1), range(1, max_procs+1), 'r--', label='Ideal')
    axes[1,0].set_xlabel('Number of Processes')
    axes[1,0].set_ylabel('Speedup')
    axes[1,0].set_title('Speedup Comparison')
    axes[1,0].legend()
    axes[1,0].grid(True)

# Plot 4: Algorithm comparison (execution time)
if len(matvec_data) > 0 and len(matmat_data) > 0:
    # Compare at common matrix sizes
    common_sizes = set(df_matvec['n'].unique()) & set(df_matmat['n'].unique())
    if common_sizes:
        comparison_data = []
        for n in common_sizes:
            mv_time = df_matvec[df_matvec['n'] == n]['time'].mean()
            mm_time = df_matmat[df_matmat['n'] == n]['time'].mean()
            comparison_data.append({'n': n, 'matvec': mv_time, 'matmat': mm_time})
        
        comp_df = pd.DataFrame(comparison_data)
        axes[1,1].bar([f'n={n}' for n in comp_df['n']], comp_df['matvec'], 
                     alpha=0.7, label='Matrix-Vector', width=0.4)
        axes[1,1].bar([f'n={n}' for n in comp_df['n']], comp_df['matmat'], 
                     alpha=0.7, label='Matrix-Matrix', width=0.4)
        axes[1,1].set_xlabel('Matrix Size')
        axes[1,1].set_ylabel('Execution Time (s)')
        axes[1,1].set_title('Algorithm Comparison')
        axes[1,1].legend()
        axes[1,1].set_yscale('log')

plt.tight_layout()
plt.savefig('hpc_a7_performance_analysis.png', dpi=300, bbox_inches='tight')
plt.show()

# Computational complexity analysis
print("\n3. COMPUTATIONAL COMPLEXITY ANALYSIS")
print("-" * 50)

print("Theoretical Complexity:")
print("  Matrix-Vector: O(n²) operations")
print("  Matrix-Matrix: O(n³) operations")

# Calculate FLOPS estimates
print("\nFLOPS Analysis:")
for algorithm in ['matvec', 'matmat']:
    subset = df[df['algorithm'] == algorithm]
    if len(subset) > 0:
        print(f"\n{algorithm.upper()}:")
        for _, row in subset.iterrows():
            if algorithm == 'matvec':
                ops = row['n'] * row['n'] * 2  # n² multiply-add operations
            else:  # matmat
                ops = row['n'] * row['n'] * row['n'] * 2  # n³ multiply-add operations
            
            flops = ops / row['time'] / 1e6  # MFLOPS
            print(f"  n={row['n']}, {row['procs']} proc: {flops:.2f} MFLOPS")

print(f"\n=== Analysis Complete ===")
print(f"Results saved to: hpc_a7_performance_analysis.png")