/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#include "tag.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (Tag);

TypeId 
Tag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Tag")
    .SetParent<ObjectBase> ()
    .SetGroupName("Network")
  ;
  return tid;
}

TypeId WifiTag::GetTypeId (void)
{
 static TypeId tid = TypeId ("ns3::WifiTag")
   .SetParent<Tag> ()
   .AddConstructor<WifiTag> ()
   .AddAttribute ("Rss",
                  "Received signal strength",
                  EmptyAttributeValue (),
                  MakeDoubleAccessor (&WifiTag::GetRssValue),
                  MakeDoubleChecker<double> ());
 return tid;
}

TypeId WifiTag::GetInstanceTypeId (void) const
{
 return GetTypeId ();
}

uint32_t WifiTag::GetSerializedSize (void) const
{
 return 8;
}

void WifiTag::Serialize (TagBuffer i) const
{
 i.WriteDouble (m_Rss);
}

void WifiTag::Deserialize (TagBuffer i)
{
 m_Rss = i.ReadDouble ();
}

void WifiTag::Print (std::ostream &os) const
{
 os << "m_Rss=" << (double)m_Rss;
}

void WifiTag::SetRssValue (double value)
{
 m_Rss = value;
}

double WifiTag::GetRssValue (void) const
{
 return m_Rss;
}

} // namespace ns3
