
namespace ArchViz.Core
{
    public interface IArchVizModule
    {
        void Initialize();
        void OnUpdate();
        void Shutdown();
    }
}