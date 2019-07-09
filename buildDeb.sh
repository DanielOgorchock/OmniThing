#!/bin/bash

rpi_tools=$HOME/programming/git/rpi-tools/
toolchain_file=Toolchain-rpi.cmake
build_target=rpi
web_config=webpage/config_rpi.json
install_root=$HOME/programming/git/cross-OmniThing/deb/omnithing
package_version="0.7-1"

cmake . -DCMAKE_TOOLCHAIN_FILE="$toolchain_file" -DRPI_TOOLS="$rpi_tools" -DBUILD_TARGET="$build_target" \
    -DWEB_CONFIG="$web_config" -DINSTALL_ROOT="$install_root" || exit

make install || exit

cd deb/omnithing || exit

mkdir -p DEBIAN
cd DEBIAN || exit

cat <<EOF > control
Package: omnithing
Architecture: all
Maintainer: Daniel Ogorchock
Priority: optional
Version: $package_version
Description: Creates SmartThings devices on raspberry pi
Depends: nodejs, pigpio
EOF

cat <<EOF > conffiles
/opt/omnithing/webpage_config.json
/opt/omnithing/config.json
EOF

cat <<EOF > postinst
#!/bin/bash
echo "Enabling and starting omnithing services..."
systemctl enable omnithing
systemctl enable omnithing-webserver
systemctl restart omnithing
systemctl restart omnithing-webserver
EOF
chmod 755 postinst

cd ../..

fakeroot dpkg-deb --build omnithing || exit

dpkg-scanpackages ./ /dev/null | gzip -9c > Packages.gz &&
apt-ftparchive release . > Release

