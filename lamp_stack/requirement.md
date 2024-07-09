# Requirements

## Install `wrk`

Update `yay`:

```
sudo pacman -S --needed base-devel
sudo pacman -Scc
sudo pacman -Syy
```

Install `wrk`:

```
yay -S wrk
```

Select `None` 

## Install python dependencies

```
sudo pacman -Syu
sudo pacman -S python python-pip
sudo pacman -S python-numpy python-pandas python-matplotlib python-seaborn
```

## Install jinja2

This package is not supported in ArchLinux python3, so we need to use python2 for this

```
pip install jinja2
```

## Generate static HTML file

```
python2.7 gen_static_html.py -s 17 -o /home/lind/lind_project/src/safeposix-rust/tmp/static.html
```
