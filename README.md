QtHashSum
=========

File Checksum Integrity Verifier & Duplicate File Finder written in C++ Qt

Download compiled Windows executable: https://github.com/fffaraz/QtHashSum/releases/latest

```
git clone https://github.com/fffaraz/QtHashSum.git
cd QtHashSum/docker
docker build --rm --no-cache -t fffaraz/qthashsum .
docker run --rm -it -v $(pwd):/data fffaraz/qthashsum .
```

### Supported algorithms

* MD4
* MD5
* SHA-1
* SHA2-224
* SHA2-256
* SHA2-384
* SHA2-512
* SHA3-224
* SHA3-256
* SHA3-384
* SHA3-512

### Screenshots

![](https://github.com/fffaraz/QtHashSum/raw/master/img/sc1.png)
![](https://github.com/fffaraz/QtHashSum/raw/master/img/sc2.png)
![](https://github.com/fffaraz/QtHashSum/raw/master/img/sc3.png)

### See Also

* [diskover](https://github.com/shirosaidev/diskover) - File system crawler, disk space usage, file search engine and file system analytics powered by Elasticsearch.
