**some useful stuff**

```bash
##Exclude Changes Temporarily This will stop Git from tracking changes to these files in your local repository.
git update-index --skip-worktree .devcontainer/devcontainer.json
git update-index --skip-worktree .vscode/settings.json

## Reverting  back
git update-index --no-skip-worktree .devcontainer/devcontainer.json
git update-index --no-skip-worktree .vscode/settings.json
```

### development dependencies
```bash
sudo apt-get update && sudo apt install g++-multilib-i686-linux-gnu gcc-multilib-i686-linux-gnu cmake git nasm grub-pc-bin grub-common mtools parted udev xorriso qemu qemu-kvm gdb -y
```

# Part 3 - Installing Build Dependencies and Getting Started

## Part 2 - Installing dependencies

We assume you have a terminal open for your Linux installation, either using WSL or similar means.

1. Install dependencies needed from the ubuntu package repository

```bash
sudo apt-get update && sudo apt install g++-multilib-i686-linux-gnu gcc-multilib-i686-linux-gnu cmake git nasm grub-pc-bin grub-common mtools parted udev xorriso qemu qemu-kvm gdb -y
```

1. Configure GIT

```nasm
$ git config --global user.name "YOUR NAME"
$ git config --global user.email your_email@uia.no
```

1. Open VSCode ikt218-osdev project in the `WSL: Ubuntu` remote

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/9c80f64b-49f2-4f67-aa85-d940454be72e/Untitled.png)

1. Now we will test if everything works:
    1. `CTRL+SHIFT+P` Select `CMake: Configure`. The output should look like this (final lines).
    
    ```nasm
    [cmake] -- Configuring done
    [cmake] -- Generating done
    [cmake] -- Build files have been written to: /home/per/ikt218-osdev/build_group_per-arne
    ```
    
    b. `CTRL+SHIFT+P` ⇒`CMake: Build Target`  ⇒ `uia-os-kernel`
    
    c. `CTRL+SHIFT+P` ⇒`CMake: Build Target`  ⇒ `uia-os-create-image`
    
2. We have now built the kernel. Now press `Start Debugging` (F5)
3. QEMU Should now start, and you should get the following error.

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/a8f7e47d-c6b9-493a-9bfd-f0578fe61be1/Untitled.png)

1. We are now done and you should git push your directory `project_per-arne`

<aside>
💡 **DO NOT PUSH ANYTHING ELSE** except `project_per-arne` and `build_project_per-arne`

</aside>

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/fc28a720-8f5b-4c61-acde-4144764c88ef/Untitled.png)

## We are now ready to develop our operating system!
