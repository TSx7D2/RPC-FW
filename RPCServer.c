class T_RPC_Server : RPCHandler
{
	override void InitialiseRPCs()
	{
		RegisterRPC(DZ_T_RPC.ON_JUMP_SERVER, ScriptCaller.Create(OnJumpRPC));
	}
	
	void OnJumpRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		Print("RPC Recieved on Server");
		
		ScriptRPC RPC = new ScriptRPC();
		RPC.Send(target, DZ_T_RPC.ON_JUMP, true);
		
		Print("RPC Sent From Server");
	}
}

