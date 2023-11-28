enum SPExecTypes
{
	CLIENT,
	SERVER,
	BOTH
}

class RPCHandler
{
	protected SPExecTypes m_SPExecType;
	protected ref map<int, ref ScriptCaller> m_RPCs = new ref map<int, ref ScriptCaller>();

	void SetExecType(SPExecTypes SPExecType)
	{
		m_SPExecType = SPExecType;
	}
	
	void InitialiseRPCs();
	
	void RegisterRPC(int rpc_type, ScriptCaller m_rpc)
	{
		ScriptCaller RPC_Caller;		
		if (m_RPCs.Find(rpc_type, RPC_Caller)) return;
		
		m_RPCs.Insert(rpc_type, m_rpc);
	}
	
	ScriptCaller FindRPC(int rpc_type)
	{
		foreach (int rpc_type_, ScriptCaller RPC: m_RPCs)
		{
			if (rpc_type_ == rpc_type) return RPC);
		}
		return null;
	}
	
	void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		#ifdef SERVER
		if (m_SPExecType == SPExecTypes.CLIENT) return;
		#else 
		if (m_SPExecType == SPExecTypes.SERVER) return;
		#endif
						
		ScriptCaller RPC = FindRPC(rpc_type);
		if (!RPC) return;
		
		RPC.Invoke(sender, target, rpc_type, ctx);
	}
}

modded class DayZGame
{
	protected ref map<string, ref RPCHandler> m_InstancedHandlers = new map<string, ref RPCHandler>();
	
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, target, rpc_type, ctx);
		
		foreach (string Classname, RPCHandler Handler: m_InstancedHandlers)
		{
			Handler.OnRPC(sender, target, rpc_type, ctx);
		}
	}
	
	void RegisterRPCHandler(typename Classname, SPExecTypes SPExecType)
	{
		#ifdef SERVER
		if (SPExecType == SPExecTypes.CLIENT) return;
		#else 
		if (SPExecType == SPExecTypes.SERVER) return;
		#endif
		
		RPCHandler Handler;
		if (m_InstancedHandlers.Find(Classname.ToString(), Handler)) return;
		
		Handler = new RPCHandler();
		Class Instance = Classname.Spawn();
		Class.CastTo(Handler, Instance);
		
		Handler.SetExecType(SPExecType);
		Handler.InitialiseRPCs();
		
		m_InstancedHandlers.Insert(Classname.ToString(), Handler);
	}
	
	void ClearRPCHandlers()
	{
		foreach (string Classname, RPCHandler Handler: m_InstancedHandlers)
		{
			delete Handler;
		}
	} 
}