use super::*;
use std::rc::Rc;

pub type Gen<'a> = Rc<&'a mut dyn generators::Generator>;
pub type Mod<'a> = Rc<&'a mut dyn modifiers::Modifier>;
pub type IntBase = dyn FnMut(StereoData, StereoData) -> StereoData;
pub type Int = Box<IntBase>;

pub struct Block<'a> {
	g: Gen<'a>,
	m: Mod<'a>,
	i: Int,
	input: StereoData,
}

impl<'a> Block<'a> {
	pub fn new<T, U>(g: &'a mut T, m: &'a mut U, i: Int) -> Block<'a>
		where T: generators::Generator + Clone,
		      U: modifiers::Modifier + Clone
	{
		Block::<'a> {
			g: Rc::new(g),
			m: Rc::new(m),
			i,
			input: StereoData::default(),
		}
	}

	pub fn default_interactor<T, U>(g: &'a mut T, m: &'a mut U) -> Block<'a>
		where T: generators::Generator + Clone,
		      U: modifiers::Modifier + Clone
	{
		Block {
			g: Rc::new(g),
			m: Rc::new(m),
			i: Box::from(|ge, mo| ge * mo),
			input: StereoData::default(),
		}
	}

	pub fn get_g(&self) -> &'a Gen {
		&self.g
	}
	pub fn get_m(&self) -> &'a Mod {
		&self.m
	}

	pub fn get_g_mut(&mut self) -> &'a mut Gen {
		&mut self.g
	}
	pub fn get_m_mut(&mut self) -> &'a mut Mod {
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
