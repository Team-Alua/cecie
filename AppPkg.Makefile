TITLE       :=  Cecie
VERSION     :=  0.10
TITLE_ID    :=  DEVP00001
CONTENT_ID  :=  IV0000-DEVP00001_00-CECIE00000000000

EXEC_NAME   := cecie.elf

ASSETS      := $(wildcard pkg/assets/**/*)
LIBMODULES  := $(wildcard pkg/sce_module/*)

TOOLCHAIN := $(OO_PS4_TOOLCHAIN)

UNAME_S     := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	CDIR := linux
endif
ifeq ($(UNAME_S), Darwin)
	CDIR := macos
endif

all: clean $(CONTENT_ID).pkg

pkg/eboot.bin: $(EXEC_NAME)
	$(TOOLCHAIN)/bin/$(CDIR)/create-fself -eboot=pkg/eboot.bin -in=$(EXEC_NAME)  --paid 0x3800000000000011

$(CONTENT_ID).pkg: pkg/pkg.gp4
	$(TOOLCHAIN)/bin/$(CDIR)/PkgTool.Core pkg_build $< .

pkg/pkg.gp4: pkg/eboot.bin pkg/sce_sys/about/right.sprx pkg/sce_sys/param.sfo pkg/sce_sys/icon0.png $(LIBMODULES) $(ASSETS)
	cd pkg && $(TOOLCHAIN)/bin/$(CDIR)/create-gp4 -out $(shell echo "$@" | sed -e 's/pkg\///g') --content-id=$(CONTENT_ID) --files "$(shell echo "$^" | sed -e 's/pkg\///g')"

pkg/sce_sys/param.sfo: Makefile
	$(TOOLCHAIN)/bin/$(CDIR)/PkgTool.Core sfo_new $@
	$(TOOLCHAIN)/bin/$(CDIR)/PkgTool.Core sfo_setentry $@ APP_TYPE --type Integer --maxsize 4 --value 1 
	$(TOOLCHAIN)/bin/$(CDIR)/PkgTool.Core sfo_setentry $@ APP_VER --type Utf8 --maxsize 8 --value '$(VERSION)'
	$(TOOLCHAIN)/bin/$(CDIR)/PkgTool.Core sfo_setentry $@ ATTRIBUTE --type Integer --maxsize 4 --value 0  
	$(TOOLCHAIN)/bin/$(CDIR)/PkgTool.Core sfo_setentry $@ CATEGORY --type Utf8 --maxsize 4 --value 'gd'  
	$(TOOLCHAIN)/bin/$(CDIR)/PkgTool.Core sfo_setentry $@ CONTENT_ID --type Utf8 --maxsize 48 --value '$(CONTENT_ID)'
	$(TOOLCHAIN)/bin/$(CDIR)/PkgTool.Core sfo_setentry $@ DOWNLOAD_DATA_SIZE --type Integer --maxsize 4 --value 0 
	$(TOOLCHAIN)/bin/$(CDIR)/PkgTool.Core sfo_setentry $@ SYSTEM_VER --type Integer --maxsize 4 --value 0  
	$(TOOLCHAIN)/bin/$(CDIR)/PkgTool.Core sfo_setentry $@ TITLE --type Utf8 --maxsize 128 --value '$(TITLE)'
	$(TOOLCHAIN)/bin/$(CDIR)/PkgTool.Core sfo_setentry $@ TITLE_ID --type Utf8 --maxsize 12 --value '$(TITLE_ID)'
	$(TOOLCHAIN)/bin/$(CDIR)/PkgTool.Core sfo_setentry $@ VERSION --type Utf8 --maxsize 8 --value '$(VERSION)'

clean:
	rm -f $(CONTENT_ID).pkg pkg/pkg.gp4 pkg/sce_sys/param.sfo pkg/eboot.bin
