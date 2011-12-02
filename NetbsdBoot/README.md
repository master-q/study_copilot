ビルド手順
==========

1. NetBSD-5.1のソースコードを入手
2. NetBSD-5.1 i386のツールチェーンだけ作成

    $ pwd
    /home/kiwamu/src/netbsd/5.1/usr/src
    $ ./build.sh -m i386 tools

3. ツールチェーンにPATHを通す

    $ ln -s /home/kiwamu/src/netbsd/5.1/usr/src/tooldir.Linux-2.6.39-2-amd64-x86_64/bin ~/tooldir_netbsd5.1
    $ echo $PATH
    /home/kiwamu/tooldir_netbsd5.1:/home/kiwamu/bin:...
    $ rehash

4. bootxxをコンパイル

    $ pwd
    /home/kiwamu/src/netbsd/5.1/src/sys/arch/i386/stand/bootxx/bootxx_cd9660
    $ nbmake-i386

5. このディレクトリに"netbsd_top"という名前でNetBSDソースへのリンクをはる

    $ pwd
    /home/kiwamu/src/study_copilot/NetbsdBoot
    $ ls -dl netbsd_top
    lrwxrwxrwx 1 kiwamu kiwamu 31 12月  2 20:27 netbsd_top -> /home/kiwamu/src/netbsd/5.1/src/

6. bootをコンパイル

    $ nbmake-i386
    $ ls -l biosboot/boot
    -rwxr-xr-x 1 kiwamu kiwamu 59884 12月  2 20:49 biosboot/boot*

7. ディスクイメージを作ってqemuを起動

    $ qemu.sh
