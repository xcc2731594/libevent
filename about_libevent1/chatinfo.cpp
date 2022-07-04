#include "chatinfo.h"

ChatInfo::ChatInfo()
{
    online_user = new list<User>;
    group_info = new list<Group>;

    mydatabase = new ChatDataBase;
    mydatabase->my_database_connect("chatgroup");
    string group_name[120];
    int group_num = mydatabase->my_database_get_group_name(group_name);
    cout<<group_num<<endl;
    for(int i=0;i<group_num;i++)
    {
        struct Group g;
        g.name = group_name[i];
        g.l = new list<GroupUser>;  //保存群中所有用户
        string member; //保存群里所有用户
        mydatabase->my_database_get_group_member(group_name[i],member);
        cout<<member<<endl;
        int start = 0, end = 0;
        GroupUser user;
        while(1)
        {
            
            string name;
            end = member.find('|',start);
            if(-1==end)
            {
                break;
            }
            user.name = member.substr(start,end-start);
            g.l->push_back(user);
            start = end+1;
            user.name.clear();
        }
        
        
        user.name = member.substr(start,member.size()-start);
        group_info->push_back(g);
    }

    // for(list<Group>::iterator it=group_info->begin();it!=group_info->end();it++)
    // {
    //     cout<<"群名字"<<it->name<<endl;
    //     for(list<GroupUser>::iterator i = it->l->begin();i!=it->l->end();i++)
    //     {
    //         cout<<i->name<<endl;
    //     }
    // }
    mydatabase->my_database_disconnect(); //关闭数据库连接
    cout<<"链表初始化成功"<<endl;
}