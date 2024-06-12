#include "data/MyData.hpp"
#include <FairFileSource.h>
#include <FairRootFileSink.h>
#include <FairRunAna.h>
#include <FairTask.h>
#include <TFile.h>
#include <TTree.h>

class MyReadTask : public FairTask
{
  public:
    explicit MyReadTask(std::string_view name)
        : FairTask{ name.data(), 1 }
    {
    }

    auto Init() -> InitStatus override
    {
        auto* root_man = FairRootManager::Instance();
        data_ = dynamic_cast<MyData*>(root_man->GetObject("MyData"));

        return kSUCCESS;
    }

    void Exec(Option_t* /*opt*/) override
    {
        if (data_ != nullptr)
        {
            std::cout << GetName() << ":\t" << "event Num: " << data_->event_num << "\t" << data_->value << "\n";
        }
    }

    void Finish() override {}

  private:
    MyData* data_ = nullptr;
};

auto main() -> int
{
    auto ana_run = FairRunAna();

    auto root_file = std::make_unique<TFile>("data.root", "read");
    auto source = std::make_unique<FairFileSource>(root_file.release());

#ifdef FAIRROOT18
    auto sink = std::make_unique<FairRootFileSink>("sink.root");
    ana_run.SetSink(sink.release());
#endif


    ana_run.SetSource(source.release());

    ana_run.AddTask(std::make_unique<MyReadTask>("1").release());
    ana_run.AddTask(std::make_unique<MyReadTask>("2").release());
    // ana_run.AddTask(std::make_unique<MyReadTask>("3"));
    // ana_run.AddTask(std::make_unique<MyReadTask>("4"));

    ana_run.Init();
    ana_run.Run(0, -1);

#ifdef FAIRROOT18
    auto* sinkfile = ana_run.GetSink();
    sinkfile->Close();
#endif
    return 0;
}
