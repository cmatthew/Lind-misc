#!/bin/bash
# Copyright (c) 2012 The Native Client Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#

# nacl-openssl-1.0.0e.sh
#
# usage:  nacl-openssl-1.0.0e.sh
#
# this script downloads, patches, and builds openssl for Native Client
#

readonly URL=http://commondatastorage.googleapis.com/nativeclient-mirror/nacl/openssl-1.0.0e.tar.gz
#readonly URL=http://www.openssl.org/source/openssl-1.0.0e.tar.gz
readonly PATCH_FILE=
readonly PACKAGE_NAME=openssl-1.0.0e

source ../../build_tools/common.sh


CustomConfigureStep() {
  Banner "Configuring ${PACKAGE_NAME}"
  ChangeDir ${NACL_PACKAGES_REPOSITORY}/${PACKAGE_NAME}

  nacl_sdk=/home/lind/tmp/lind/sdk/linux_x86/

  MACHINE=linux-x86_64
  CC=nacl64-gcc 
  AR=$nacl_sdk/bin/nacl-ar
  RANLIB=$nacl_sdk/bin/nacl-ranlib
  ./config --prefix=$nacl_sdk no-asm no-hw no-krb5 -D_GNU_SOURCE
}


CustomBuildStep() {
  make clean
  make build_libs
}


CustomPackageInstall() {
  DefaultPreInstallStep
  DefaultDownloadStep
  DefaultExtractStep
  # openssl doesn't need patching, so no patch step
  CustomConfigureStep
  CustomBuildStep
  DefaultInstallStep
  DefaultCleanUpStep
}

CustomPackageInstall
exit 0
