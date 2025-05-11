import time
import subprocess
import os
import platform
import matplotlib.pyplot as plt
measured_times = {
    "python": 0.0,
    "c_cmp": 0.0,
    "c_run": 0.0
}


def run_python():
    print("measuring python runtime")
    tick = time.time();
    subprocess.run(["python3", "python/main.py"], check=True)
    tock = time.time();
    measured_times["python"] = tock-tick
    print(f"Python runtime: {measured_times['python']:.6f}s\n")

def run_c():
    print("measuring c compile time")
    if os.path.exists("./main.exe"):
        os.remove("./main.exe")
    tick = time.time();
    subprocess.run(
        ["clang", "-o", "main",
         "c/main.c", "c/cpu.c", "c/instruction.c"],
        check=True)
    tock = time.time();
    measured_times["c_cmp"] = tock-tick
    print(f"C compile time: {measured_times['c_cmp']:.6f}s\n")
    print("measuring c runtime");
    tick = time.time()
    subprocess.run([f"./main"] if platform.system() != "Windows" else [f"./main.exe"])
    tock = time.time()
    measured_times["c_run"] = tock-tick;
    print(f"C runtime: {measured_times['c_run']:.6f}s\n")

run_python()
run_c()

labels = ["Python Run", "C Compile", "C Run"]
values = [measured_times["python"], measured_times["c_cmp"], measured_times["c_run"]]
colors = ["tab:blue", "tab:orange", "tab:green"]

fig, ax = plt.subplots()
ax.bar(labels, values, color=colors)
ax.set_ylabel("Time (seconds)")
ax.set_title("CPU Simulator: Python vs. C (compile + run)")
for i, v in enumerate(values):
    ax.text(i, v + max(values)*0.01, f"{v:.3f}s", ha="center")
plt.tight_layout()
plt.savefig("cpu_sim_benchmark.png")
plt.show()
