ORIGIN=/home/lopespt/Dropbox/Shares\ PS/q-SIFT/Datasets
TEMP_FOLDER=../RunningExperiment/
THIS_DIR=$(pwd)

if [ -d $TEMP_FOLDER ]; then
    rm -rf $TEMP_FOLDER
fi

mkdir -p $TEMP_FOLDER
cd $TEMP_FOLDER
pwd


#CAMVID
ln -s "$ORIGIN"/camvid/ camvid

#ImageSequences
mkdir ImageSequences
cd ImageSequences
for arq in "$ORIGIN/ImageSequences/"*.zip; do
    #unzip -d u -j "$ORIGIN/ImageSequences/*.zip" *.jpg
    nomeArq=$(basename "$arq"|cut -d "." -f1)
    unzip -d $nomeArq -j "$arq" *.jpg
    N=100
    for carq in $(ls $nomeArq); do
        ((i=i%N)); ((i++==0)) && wait
        ( if file "$nomeArq"/$carq | grep "image data" > /dev/null; then
                python "$THIS_DIR"/formataArquivos.py "$nomeArq"/$carq && rm "$nomeArq"/$carq
            fi;
        ) &

    done;

#unzip "$ORIGIN/ImageSequences/*.zip"
done;
cd ..

#
python ../Python/execute_experiment.py





