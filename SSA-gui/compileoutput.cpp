#include "compileoutput.h"
#include <QDebug>
#include <cstdio>
#include "SSA-core/driver.hh"
#include <iostream>
#include <fstream>
#include "error-handling/gui_error_handler.hh"
#include "SSA-core/code_generator.hh"
#include "maindocument.h"

//extern "C"
//{
//#include "gen/java_parser.h"
//#include "gen/java_lexer.h"
//extern bool build_success;
//void reset_all();
//}
//constexpr int  MAX_ERR_SIZE = 1<<20,MAX_ERR_LINE=256;

CompileOutput::CompileOutput(QWidget *parent)
    : QTextEdit{parent}
{
    this->setReadOnly(true);
    QPalette darkPalette;
QColor darkColor(45, 45, 45);
QColor textColor(220, 220, 220);
darkPalette.setColor(QPalette::Window, darkColor);
darkPalette.setColor(QPalette::WindowText, textColor);
darkPalette.setColor(QPalette::Base, QColor(30, 30, 30));
darkPalette.setColor(QPalette::AlternateBase, darkColor);
darkPalette.setColor(QPalette::ToolTipBase, textColor);
darkPalette.setColor(QPalette::ToolTipText, textColor);
darkPalette.setColor(QPalette::Text, textColor);
darkPalette.setColor(QPalette::Button, darkColor);
darkPalette.setColor(QPalette::ButtonText, textColor);
darkPalette.setColor(QPalette::BrightText, Qt::red);
darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
darkPalette.setColor(QPalette::HighlightedText, Qt::black);

this->setPalette(darkPalette);
}

bool CompileOutput::runCompilation(QString path) {


    javacompiler::GuiErrorHandler errorHandler;
    errorHandler.setTextEdit(*this);
    javacompiler::Driver driver(errorHandler);

    bool result = false;

    QStringList parts = path.split("/");
    std::string filename = parts.at(parts.size()-1).toStdString();

    std::string filepath = path.toStdString();

    std::fstream infile(filepath);

    if (!infile.good()) {
        std::cerr << "Could not open file: " << filepath << std::endl;
        return 0;
    }

    try {
        result = driver.parse_stream(infile, filename);
        QString binCode=QString::fromStdString(driver.binaryCode);
        QDialog dialog;
        auto layout=new QVBoxLayout;
        dialog.setLayout(layout);
        MainDocument *generatedCode = new MainDocument(&dialog,false);
        layout->addWidget(generatedCode);
        generatedCode->setReadOnly(true);
        dialog.setWindowTitle(tr("Generated Code"));
        generatedCode->setPlainText(binCode);
        dialog.exec();
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(...) {
        return false;
    }

    return result;



//    if(!runParser(path))
//    {
//        setMarkdown(toMarkdown()+tr(R"(
//<span style="color:red">**Build failed**</span>)"));
//        return false;
//    }
//    /*
//     * Only to debug parser
//    */
//    else{
//        setMarkdown(tr(R"(<span style="color:green">**Build is successful**</span>)"));
//        return true;
//    }
//    QString program = "javac";
//    QStringList arguments;
//    arguments << path;

//    QProcess* process = new QProcess(this);
//    process->start(program, arguments);
//    qDebug() << program << " "<< arguments;
//    if (!process->waitForFinished()) // This is blocking and needs to be called within a thread
//    {
//        this->setText(tr("Blocking Error"));
//        return false;
//    }
//    QString output = process->readAllStandardError();
//    if (output.isEmpty()) { // no errors
//        output = "Program compiled successfully.";
//    }
//    this->setText(output);
//    return true;
}

/*
 * TODO: Configure Bison & Flex to generate C++ code
*/
bool CompileOutput::runParser(QString path)
{
//    setMarkdown("");
//    FILE *file=fopen(path.toStdString().c_str(),"r"),*tmpFile=tmpfile64();
//    static char errOut[MAX_ERR_SIZE],errLine[MAX_ERR_LINE];
//    char*errPtr=errOut;
//    errOut[0]=0;
//    if(file && tmpFile)
//    {
//        yyin=file;
//        yyout=tmpFile;
//        yyparse();
//        bool err=build_success;
//        reset_all();
//        qDebug() << "Finished Parsing";
//        rewind(yyout);
//        while(fgets(errLine,MAX_ERR_LINE,yyout))
//        {
//            strcat(errOut,errLine);
//            strcat(errOut,"\n");
//        }
//        setMarkdown(errOut);
//        yylex_destroy();
//        return err;
//    }
    return false;
}

