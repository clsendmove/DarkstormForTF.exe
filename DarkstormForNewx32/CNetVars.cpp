#include <utility>
#include "CNetVars.h"
#include "SDK.h"
#undef GetProp

/**
* netvar_tree - Constructor
*
* Call populate_nodes on every RecvTable under client->GetAllClasses()
*/
void CNetVars::Initialize()
{
	const auto* client_class = gInts.Client->GetAllClasses();
	while (client_class != nullptr) {
		const auto class_info = std::make_shared<node>(0);
		auto* recv_table = client_class->Table;
		populate_nodes(recv_table, &class_info->nodes);
		nodes.emplace(recv_table->GetName(), class_info);

		client_class = client_class->pNextClass;
	}
}

/**
* populate_nodes - Populate a node map with brances
* @recv_table:	Table the map corresponds to
* @map:	Map pointer
*
* Add info for every prop in the recv table to the node map. If a prop is a
* datatable itself, initiate a recursive call to create more branches.
*/
void CNetVars::populate_nodes(RecvTable* recv_table, map_type* map)
{
	for (auto i = 0; i < recv_table->GetNumProps(); i++) {
		const auto* prop = recv_table->GetProp(i);
		const auto prop_info = std::make_shared<node>(prop->GetOffset());

		if (prop->GetType() == DPT_DataTable)
			populate_nodes(prop->GetDataTable(), &prop_info->nodes);

		map->emplace(prop->GetName(), prop_info);
	}
}


int CNetVars::GetOffset(RecvTable* pTable, const char* szNetVar)
{
	for (int i = 0; i < pTable->m_nProps; i++)
	{
		RecvProp Prop = pTable->m_pProps[i];

		if (std::string_view(Prop.m_pVarName).compare(szNetVar) == 0)
			return Prop.GetOffset();

		if (auto DataTable = Prop.GetDataTable())
		{
			if (auto nOffset = GetOffset(DataTable, szNetVar))
				return nOffset + Prop.GetOffset();
		}
	}

	return 0;
}
int CNetVars::GetNetVar(const char* szClass, const char* szNetVar)
{
	ClientClass* pClasses = gInts.Client->GetAllClasses();

	for (auto pCurrNode = pClasses; pCurrNode; pCurrNode = pCurrNode->pNextClass)
	{
		if (std::string_view(szClass).compare(pCurrNode->chName) == 0)
			return GetOffset(pCurrNode->Table, szNetVar);
	}

	return 0;
}