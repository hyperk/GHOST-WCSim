######################################################################
### Starting point
######################################################################
### Base image
### Assumes a base image with compilers, python, and hk-pilot installed.
### Using hk-software so that Root and Geant4 do not need to be recompiled.
FROM ghcr.io/hyperk/hk-software:latest

### Copying the current working directory is common for HyperK Dockerfiles
#COPY . /usr/local/hk/hk-ToolApp

### Keyscanning the repository is typical for HyperK Dockerfiles.
### Already done in the base image.
RUN --mount=type=ssh mkdir -p -m 0600 ~/.ssh && ssh-keyscan github.com >> ~/.ssh/known_hosts

ENV HYPERKDIR /usr/local/hk

######################################################################
### Update hk-pilot
######################################################################
WORKDIR $HYPERKDIR/hk-pilot

### ------------------------------------------------------------------
### 1. Update hk-pilot from repository 
### REMOVE WHEN BASE IMAGE IS UPDATED
###
### The first command clears the base image creator's git authentication
### so that the authentication of the current image creator can be used.
### ------------------------------------------------------------------
RUN --mount=type=ssh git config --local --unset http.https://github.com/.extraheader \
        && git restore . \
        && git remote add temporary git@github.com:hyperk/hk-pilot.git \
        && git fetch temporary main \
        && git branch main --set-upstream-to temporary/main \
        && git pull

### ------------------------------------------------------------------
### 2. Set up to pull projectes from development repositories
### ToDo:  Use variables?  Or just remove?  This should be temporary
### ------------------------------------------------------------------
RUN echo "  hk-DataModel: git@github.com:tdealtry/hk-DataModel.git" >> /usr/local/hk/hk-pilot/data/repos.yaml \
        && echo "  hk-ToolApp: git@github.com:tdealtry/hk-ToolApp.git" >> /usr/local/hk/hk-pilot/data/repos.yaml

### ------------------------------------------------------------------
### 3. Update installation
### ------------------------------------------------------------------
RUN --mount=type=ssh python3 install.py


######################################################################
### Update WCSim configuration
###
### This should be a temporary step that is eventually removed
### Sets up to pull a development branch of the WCSim source code
######################################################################
WORKDIR $HYPERKDIR/WCSim

RUN sed --in-place "s/WCSim\/WCSim.git/tdealtry\/WCSim.git/" /usr/local/hk/WCSim/hkinstall.py \
        && sed --in-place 's/_git_checkout_tag = ".*"/_git_checkout_branch = "ghost-devel"/' /usr/local/hk/WCSim/hkinstall.py

# Clean the image-native version of WCSim
WORKDIR $HYPERKDIR/hk-pilot
RUN --mount=type=ssh . $HYPERKDIR/hk-pilot/setup.sh \
        && hkp clean --deep WCSim


######################################################################
### Update ToolFrameWorkCore
######################################################################
WORKDIR $HYPERKDIR/ToolFrameworkCore
RUN --mount=type=ssh git pull

# Clean the image-native version of ToolFrameWorkCore and download recent
WORKDIR $HYPERKDIR/hk-pilot
RUN --mount=type=ssh . $HYPERKDIR/hk-pilot/setup.sh \
        && hkp clean --deep ToolFrameworkCore \
        && hkp install ToolFrameworkCore

### TEMPORARY:  Remove when the name conflict is fixed
WORKDIR $HYPERKDIR/ToolFrameworkCore
RUN sed --in-place 's!gray!grey!' /usr/local/hk/ToolFrameworkCore/src/src/Logging/Logging.h


######################################################################
### Download and update missing projects
###
### TEMPORARY
### Only needed on development branches
######################################################################
WORKDIR $HYPERKDIR/hk-pilot

RUN --mount=type=ssh . $HYPERKDIR/hk-pilot/setup.sh \
        && hkp download hk-DataModel \
        && hkp download hk-testTool
#        && hkp download GHOST-WCSim  # See treatment of GHOST-WCSim below
#        && hkp download hk-ToolApp   # See treatment of ToolApp below

### ------------------------------------------------------------------
WORKDIR $HYPERKDIR/hk-DataModel
RUN --mount=type=ssh git checkout --track origin/ghost-devel


### ------------------------------------------------------------------
### Get GHOST-WCSim and hk-ToolApp
###
### TEMPORARY
### Remove when default branches of GHOST-WCSim and hk-ToolApp have
### hk-pilot integration
### ------------------------------------------------------------------
WORKDIR $HYPERKDIR
RUN --mount=type=ssh git clone --branch hk-toolapp git@github.com:hyperk/GHOST-WCSim.git \
        && git clone --branch ghost-devel git@github.com:tdealtry/hk-ToolApp.git

### ------------------------------------------------------------------
### Update dependencies of hk-testTool
###
### TEMPORARY!
###
### The update hk-DataModel has a dependence on Geant4, which makes
### hk-testTool *indirectly* dependent on Geant4.  For some reason,
### indirect dependencies are not handled intuitively by the current
### hk-pilot system.  Geant4 must be explicitely added to the
### dependencies of hk-testTool.
### ------------------------------------------------------------------
WORKDIR $HYPERKDIR/hk-pilot
RUN echo 'hk_package( Geant4 *)' >> $HYPERKDIR/hk-testTool/dependencies.cmake


######################################################################
### Install packages!
######################################################################
WORKDIR $HYPERKDIR/hk-pilot
RUN --mount=type=ssh . $HYPERKDIR/hk-pilot/setup.sh \
        && hkp install --recursive hk-ToolApp


