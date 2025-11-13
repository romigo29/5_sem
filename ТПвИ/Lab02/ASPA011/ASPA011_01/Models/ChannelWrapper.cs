using System.Threading.Channels;

namespace ASPA011_01.Models
{
	public class ChannelWrapper
	{
		public Guid Id { get; } = Guid.NewGuid();
		public string Name { get; set; } = string.Empty;
		public string Description { get; set; } = string.Empty;
		public ChannelState State { get; set; } = ChannelState.ACTIVE;

		public Channel<string> Queue { get; } = Channel.CreateUnbounded<string>();

		public DateTime CreatedAt { get; } = DateTime.UtcNow;
	}
}
