#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QRegularExpression>
#include <QString>
#include <QVector>
#include <VideoAsift.hpp>
#include <algorithm>
#include <functional>
#include <random>
#include <tuple>

struct ExperimentFrame {
  int num;
  FrameInfo features;
};

vector<vector<float>> distancias(vector<ExperimentFrame> &list) {

  vector<vector<float>> d(list.size());
  
  
  for (unsigned int i = 0; i < d.size(); i++) {
    d[i].resize(list.size());
  }

  for (unsigned int i = 0; i < d.size(); i++) {
    for (unsigned int j = 0; j < d.size(); j++) {
      // cout << i << " " << j << endl;
      matchingslist mlist;
      siftPar parameters;
      default_sift_parameters(parameters);
      compute_asift_matches(
          5, 5, list[i].features.w, list[i].features.h, list[j].features.w,
          list[j].features.h, 0, list[i].features.keypoints,
          list[j].features.keypoints, mlist, parameters, &d[i][j]);
    }
  }

  return d;
}

tuple<int, int, QString> discoverFramesInFolder(QString folder) {
  QFileInfo ff(folder);
  QFileInfoList imageFiles = ff.dir().entryInfoList();
  QVector<int> indexes;
  QRegularExpression re("[-_](\\d*)\\.\\w*$");
  QString basename;
  for (auto fi : imageFiles) {
    QRegularExpressionMatch m = re.match(fi.fileName());
    if (m.hasMatch()) {
      indexes.append(m.captured(1).toInt());
      basename = fi.fileName().replace(m.captured(1) + ".png", "");
    }
  }
  qSort(indexes);

  return tuple<int, int, QString>(indexes.first(), indexes.last(), basename);
};

float avalia(vector<int> result) {
  float res = 0;
  for (unsigned int i = 2; i < result.size(); i++) {
    res += (result[i - 1] < result[i]) ? 1 : -1;
  }
  // return res;
  return (res + float(result.size() - 1)) / (2.0 * float(result.size()) - 2.0);
}

void runExperiment(QString folder, int from = 0, int to = 0, int step = 1) {
  int foundFrom;
  int foundTo;
  QString pattern;

  tie(foundFrom, foundTo, pattern) = discoverFramesInFolder(folder);

  QStringList sl = folder.split("/");
  sl.removeLast();
  sl.append(pattern);
  pattern = sl.join("/");

  from = from == 0 ? foundFrom : from;
  to = to == 0 ? foundTo : to;

  VideoASift vid(pattern.toStdString(), from, to);
  vector<ExperimentFrame> frames;
  for (int i = from; i < to; i += step) {
    ExperimentFrame frame;
    frame.num = i;
    //frame.features = vid.extractASiftFeatures(i);
    frame.features = vid.extractASiftFeaturesMemoized(i);
    frames.push_back(frame);
  }

  std::default_random_engine gen;
  shuffle(frames.begin() + 1, frames.end(), gen);

  vector<vector<float>> d = distancias(frames);
  vector<int> framesDisponiveis(d.size() - 1);

  iota(framesDisponiveis.begin(), framesDisponiveis.end(), 1);
  vector<int> ordem;
  ordem.push_back(0);

  for (unsigned int i = 1; i < d.size(); i++) {
    int m = framesDisponiveis[0];
    for (unsigned int fidx : framesDisponiveis) {
      // quanto menor d, mais próximo
      if (i != fidx && d[i][fidx] < d[i][m]) {
        m = fidx;
      }
    }
    ordem.push_back(m);
    remove(framesDisponiveis.begin(), framesDisponiveis.end(), m);
    framesDisponiveis.pop_back();
  }
  float resultado = avalia(ordem);
  cout << step << "\t" << resultado << endl;
}

int main(int argc, char **argv) {
  QCoreApplication app(argc, argv);

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addPositionalArgument("folder", "Images Folder");
  parser.addOption(QCommandLineOption(QStringList{"from", "f"},
                                      "Starts at frame f", "from"));
  parser.addOption(
      QCommandLineOption(QStringList{"to", "t"}, "Ends at frame t", "to"));
  parser.addOption(
      QCommandLineOption(QStringList{"step", "s"}, "Frame steps", "step"));
  parser.process(app);

  int from = parser.isSet("f") ? parser.value("f").toInt() : 0;
  int to = parser.isSet("t") ? parser.value("t").toInt() : 0;
  int step = parser.isSet("s") ? parser.value("s").toInt() : 1;

  runExperiment(parser.positionalArguments().value(0), from, to, step);
  // printf("Checkpoint %d\n", 1);
  /*
     float q = parser.positionalArguments().value(1).toFloat();
     float b = parser.positionalArguments().value(2).toFloat();
     int from, to, step=1;
     QString basename;
     tie(from, to, basename) =
     discoverFramesInFolder(parser.positionalArguments().value(0)); basename =
     parser.positionalArguments().value(0) + basename;

  // printf("Checkpoint %d\n", 2);

  if(parser.isSet("from"))
  from = parser.value("from").toInt();
  if(parser.isSet("to"))
  to = parser.value("to").toInt();
  if(parser.isSet("step"))
  step = parser.value("step").toInt();

*/

  return 0;
}
