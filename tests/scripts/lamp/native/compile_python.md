## Uninstall native ubuntu python

```sh
sudo apt-get remove --purge python2.* python3.*
sudo apt-get remove --purge python3-pip python3-venv python3-*
sudo apt-get autoremove --purge
sudo apt-get autoclean
```

```sh
export LD_LIBRARY_PATH=/usr/local/python-gcc4/lib:/usr/local/pgsql/lib:$LD_LIBRARY_PATH
export PYTHONHOME="/usr/local/python-gcc4"
export PYTHONPATH="/usr/local/python-gcc4/lib/:/usr/local/python-gcc4/lib/python2.7/site-packages"
export PATH=/usr/local/python-gcc4/bin/:$PATH
```