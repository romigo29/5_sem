namespace ASPA011_01.Models
{

	public enum ChannelState { ACTIVE, CLOSED }
	public enum QueueOperations { ENQUEUE, DEQUEUE, PEEK }
	public class QueueSettings
	{
		public int WaitEnqueue { get; set; } = 5; // по умолчанию 5 секунд
	}
	public class ChannelInfo
	{
		public Guid Id { get; set; }
		public string Name { get; set; } = default!;
		public ChannelState State { get; set; }
		public string Description { get; set; } = default!;
	}

	public class CreateChannel
	{
		public string Command { get; set; } = "new";
		public string Name { get; set; } = default!;
		public ChannelState State { get; set; }
		public string Description { get; set; } = string.Empty;
	}

	public class CloseAllChannels
	{
		public string Command { get; set; } = "close";
		public string Reason { get; set; } = string.Empty;
	}

	public class CloseChannelById
	{
		public Guid Id { get; set; }
		public string Command { get; set; } = "close";
		public string Reason { get; set; } = string.Empty;

	}

	public class OpenAllChannels
	{
		public string Command { get; set; } = "open";
	}

	public class OpenChannelById
	{
		public Guid Id { get; set; }
		public string Command { get; set; } = "open";
		public ChannelState State { get; set; } = ChannelState.ACTIVE;
	}


	public class DeleteAllChannels
	{
		public string Command { get; set; } = "delete";
	}

	public class DeleteClosedChannels
	{
		public string Command { get; set; } = "delete";
		public ChannelState State { get; set; } = ChannelState.CLOSED;
	}

	public class EnqueueElement
	{
		public string Command { get; set; } = "enqueue"; // было QueueOperations
		public Guid Id { get; set; }
		public string Data { get; set; } = default!;
	}

	public class DequeueElement
	{
		public string Command { get; set; } = default!; // было QueueOperations
		public Guid Id { get; set; }
	}

	public class Element
	{
		public Guid Id { get; set; }
		public string Data { get; set; } = default!; //JSON string
	}

	public class ErrorMessage
	{
		public Guid Id { get; set; }
		public string Error { get; set; } = default!;
	}

}
