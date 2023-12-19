# jar-to-dll

A simple tool that allows you to pack your mod .jar file to .dll and inject it into running Minecraft

## Simple tutorial

[English](/for-dummies/FOR_DUMMIES_EN.md) | [На русском](/for-dummies/FOR_DUMMIES_RU.md)

## Usage

For building this I personally recommend using WSL on Windows or Linux

To build, you need these packages installed:

- `g++-mingw-w64-x86-64`
- Some kind of JDK (I prefer using [Adoptium Temurin](https://adoptium.net/temurin/releases/), all install instructions are available on their website)

Then, you need to drop your mod .jar as `input.jar` into the root directory of repo

And as a last step - just build it:
```shell
make build
```

As a result you'll get `output.dll` which you can inject

## Supported versions and features

- Only Forge mods are supported, by now
- Only mods that do not require usage of URLClassLoader to do something dynamically
- Your mod should do all stuff inside no-args constructor in class that is marked with `@Mod` annotation
- Tested on 1.7.10, 1.8.8, 1.12.2, 1.16.5

