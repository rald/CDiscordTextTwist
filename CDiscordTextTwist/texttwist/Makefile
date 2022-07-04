texttwist: texttwist.c texttwist.h util.h
	cc texttwist.c -o texttwist -I. -Iconcord/include -Iconcord/core -Iconcord/core/third-party -Iconcord/gencodecs -Iconcord/cog-utils -L. -Lconcord/lib -pthread -ldiscord -lcurl
.PHONY: clean

clean:
	rm texttwist
