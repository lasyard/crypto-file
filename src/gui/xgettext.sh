#!/usr/bin/env sh

DIR="res/i18n"

if [ ! -d "${DIR}" ]; then
    mkdir -p "${DIR}"
fi

OPTIONS="--copyright-holder=Lasy
--package-name=crypto-file-viewer
--package-version=1.0.0
--msgid-bugs-address=lasyard@yeah.net"

xgettext --c++ -k, -k_ -kt -f "xgettext.list" -p "${DIR}" -o CryptoFileViewer.pot ${OPTIONS}

wxrc -g res/*.xrc | xgettext --c++ -k, -k_ -p "${DIR}" -o xrc.pot - ${OPTIONS}

for domain in "CryptoFileViewer" "xrc"; do
    for locale in "en" "zh"; do
        file="${DIR}/${domain}_${locale}.po"
        if [ -f "${file}" ]; then
            msgmerge -U "${file}" "${DIR}/${domain}.pot"
        else
            msginit -i "${DIR}/${domain}.pot" -o "${file}" --locale ${locale} --no-translator --no-wrap
        fi
    done
done
