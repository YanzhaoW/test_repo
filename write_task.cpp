#include "data/MyData.hpp"
#include <FairRootFileSink.h>
#include <FairRunAna.h>
#include <FairTask.h>
#include <TFile.h>
#include <TRandom3.h>
#include <TTree.h>

class MyReadTask : public FairTask
{
  public:
    MyReadTask()
        : FairTask{ "MyWriteTask", 1 }
    {
        data_.SetName("MyData");
        ran_gen_.SetSeed();
    }

    auto Init() -> InitStatus override
    {
        auto* root_man = FairRootManager::Instance();
        root_man->Register(data_.GetName(), "data", &data_, true);

        return kSUCCESS;
    }

    void Exec(Option_t* /*opt*/) override
    {
        data_.event_num = event_count_;
        data_.value = ran_gen_.Uniform(0., 10.);

        ++event_count_;
    }

    void Finish() override {}

  private:
    int event_count_ = 0;
    MyData data_;
    TRandom3 ran_gen_;
};

auto main() -> int
{
    auto ana_run = FairRunAna();

    auto root_file = std::make_unique<TFile>("data.root", "RECREATE");
    auto sink = std::make_unique<FairRootFileSink>(root_file.release());

    ana_run.SetSink(std::move(sink));

    ana_run.AddTask(std::make_unique<MyReadTask>().release());

    ana_run.Init();
    ana_run.Run(0, 100);

#ifdef FAIRROOT18
    auto* sinkfile = ana_run.GetSink();
    sinkfile->Close();
#endif

    return 0;
}
