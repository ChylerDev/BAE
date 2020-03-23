//! # Mixer

use super::*;

use std::collections::VecDeque;
use crate::channels::{Channel, ChannelSP};
use crate::sounds::BlockSP;

pub struct Mixer {
    modifier_list: Vec<BlockSP>,
    channel_list: VecDeque<ChannelSP>,
}

impl Mixer {
    pub fn modifier_list(&self) -> &Vec<BlockSP> {
        &self.modifier_list
    }

    pub fn modifier_list_mut(&mut self) -> &mut Vec<BlockSP> {
        &mut self.modifier_list
    }

    pub fn channel_list(&self) -> &VecDeque<ChannelSP> {
        &self.channel_list
    }

    pub fn channel_list_mut(&mut self) -> &mut VecDeque<ChannelSP> {
        &mut self.channel_list
    }
}
