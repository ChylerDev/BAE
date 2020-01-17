use super::*;
use std::rc::Rc;

pub type Gen = Rc<dyn generators::Generator>;
pub type Mod = Rc<dyn modifiers::Modifier>;
pub type IntBase = dyn FnMut(StereoData, StereoData) -> StereoData;
pub type Int = Box<IntBase>;

pub struct Block {
	g: Gen,
	m: Mod,
	i: Int,
	input: StereoData,
}

impl Block {
	pub fn new<T, U>(g: T, m: U, i: Int) -> Block 
		where T: 'static + generators::Generator + Clone,
		      U: 'static + modifiers::Modifier + Clone
	{
		Block {
			g: Rc::new(g),
			m: Rc::new(m),
			i,
			input: StereoData::default(),
		}
	}
	pub fn default_interactor<T, U>(g: T, m: U) -> Block 
		where T: 'static + generators::Generator + Clone,
		      U: 'static + modifiers::Modifier + Clone
	{
		Block {
			g: Rc::new(g),
			m: Rc::new(m),
			i: Box::from(|ge, mo| ge * mo),
			input: StereoData::default(),
		}
	}

	pub fn get_g(&self) -> &Gen {
		&self.g
	}
	pub fn get_m(&self) -> &Mod {
		&self.m
	}

	pub fn get_g_mut(&mut self) -> &mut Gen {
		&mut self.g
	}
	pub fn get_m_mut(&mut self) -> &mut Mod {
		&mut self.m
	}

	pub fn prime_input(&mut self, x: StereoData) {
		self.input += x;
	}

	pub fn process(&mut self) -> StereoData {
		let y = (*self.i)(
			Rc::get_mut(&mut self.g).unwrap().process(),
			Rc::get_mut(&mut self.m).unwrap().process(self.input)
		);

		self.input = StereoData::default();

		y
	}
}
