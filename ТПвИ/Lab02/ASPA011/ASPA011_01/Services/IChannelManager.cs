using ASPA011_01.Models;

namespace ASPA011_01.Services
{
	public interface IChannelManager
	{
		IEnumerable<ChannelInfo> GetAll();
		ChannelInfo? Get(Guid id);
		ChannelInfo Create(CreateChannel model);
		bool CloseAll(string reason);
		bool CloseById(Guid id, string reason);
		bool OpenAll();
		bool OpenById(Guid id);
		bool DeleteAll();
		bool DeleteClosed();
		Element? Enqueue(Guid id, string data);
		Element? Dequeue(Guid id);
		Element? Peek(Guid id);
	}
}
