class T_RPC_Client : RPCHandler
{
	override void InitialiseRPCs()
	{
		RegisterRPC(DZ_T_RPC.ON_JUMP, ScriptCaller.Create(OnJumpRPC));
	}
	
	void OnJumpRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		Print("RPC Recieved On Client");
	}
}


