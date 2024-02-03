# Cycraft Emulator

This is an emulator for the Triforce game F-Zero AX Monster Ride which is compatible with the Simuline Cycraft.

It may also work with the Naomi 2 games Initial D3 and Club Kart with are also compatible.

The protocol was reverse engineered by studying the simulator's controller software which was dumped from the computer inside the simulator.

## Building & Running

The software is targeted at Linux. Simply install the build-essential package and use gcc to compile the cycraft executable and run.

```
sudo apt install build-essential
gcc cycraft.c -o cycraft
./cycraft
```

You must make sure your computers IP Address is set to `192.168.11.111` and that your set to Group A ID 1 in the Cycraft settings menu.

## Windows Version

Thanks to RεtroFΔn⊗ there is now a version converted for windows named `cycraft-windows.c`.
