ビルド手順
==========

Debian sid amd64 2011年12月02日時点ぐらいで試験。

1. http://leepike.github.com/Copilot/ をインストールしておく
2. NetBSD-5.1のソースコードを入手

<pre>
    $ pwd
    /home/kiwamu/src/netbsd/5.1
    $ cvs -d :pserver:anoncvs@anoncvs.netbsd.org:/cvsroot login
    $ cvs -d :pserver:anoncvs@anoncvs.netbsd.org:/cvsroot co -r netbsd-5-1-RELEASE src
</pre>

3. NetBSD-5.1 i386のツールチェーンだけ作成

<pre>
    $ pwd
    /home/kiwamu/src/netbsd/5.1/src
    $ ./build.sh -m i386 tools
</pre>

4. ツールチェーンにPATHを通す

<pre>
    $ ln -s /home/kiwamu/src/netbsd/5.1/src/tooldir.Linux-2.6.39-2-amd64-x86_64/bin ~/tooldir_netbsd5.1
    $ echo $PATH
    /home/kiwamu/tooldir_netbsd5.1:/home/kiwamu/bin:...
    $ rehash
</pre>

5. bootxxをコンパイル

<pre>
    $ pwd
    /home/kiwamu/src/netbsd/5.1/src/sys/arch/i386/stand/bootxx/bootxx_cd9660
    $ nbmake-i386
</pre>

6. このディレクトリに"netbsd_top"という名前でNetBSDソースへのリンクをはる

<pre>
    $ pwd
    /home/kiwamu/src/study_copilot/NetbsdBoot
    $ ls -dl netbsd_top
    lrwxrwxrwx 1 kiwamu kiwamu 31 12月  2 20:27 netbsd_top -> /home/kiwamu/src/netbsd/5.1/src/
</pre>

7. bootをコンパイル

<pre>
    $ pwd
    /home/kiwamu/src/study_copilot/NetbsdBoot
    $ nbmake-i386
    $ ls -l biosboot/boot
    -rwxr-xr-x 1 kiwamu kiwamu 59884 12月  2 20:49 biosboot/boot*
</pre>

8. ディスクイメージを作ってqemuを起動

<pre>
    $ ./qemu.sh
</pre>
