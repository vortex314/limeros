use serde_json::{Map, Value as JsonValue};
use std::collections::HashMap;
use std::ops::Index;

/// Dynamic value type for polymorphic data (JSON/CBOR decoding).
/// Renamed from `Poly` to avoid collision with the proto-generated
/// `Poly` message type in `msgs.rs`.
#[derive(Clone, Debug, PartialEq)]
pub enum DynVal {
    Null,
    Bool(bool),
    Signed(i64),
    Unsigned(u64),
    Float(f64),
    String(String),
    Array(Vec<DynVal>),
    Object(HashMap<String, DynVal>),
}

impl Default for DynVal {
    fn default() -> Self {
        Self::Null
    }
}

impl DynVal {
    pub fn array() -> Self {
        Self::Array(Vec::new())
    }

    pub fn object() -> Self {
        Self::Object(HashMap::new())
    }

    pub fn is_null(&self) -> bool {
        matches!(self, Self::Null)
    }

    pub fn is_bool(&self) -> bool {
        matches!(self, Self::Bool(_))
    }

    pub fn is_signed(&self) -> bool {
        matches!(self, Self::Signed(_))
    }

    pub fn is_unsigned(&self) -> bool {
        matches!(self, Self::Unsigned(_))
    }

    pub fn is_float(&self) -> bool {
        matches!(self, Self::Float(_))
    }

    pub fn is_number(&self) -> bool {
        self.is_signed() || self.is_unsigned() || self.is_float()
    }

    pub fn is_string(&self) -> bool {
        matches!(self, Self::String(_))
    }

    pub fn is_array(&self) -> bool {
        matches!(self, Self::Array(_))
    }

    pub fn is_object(&self) -> bool {
        matches!(self, Self::Object(_))
    }

    pub fn size(&self) -> usize {
        match self {
            Self::Array(v) => v.len(),
            Self::Object(v) => v.len(),
            _ => 0,
        }
    }

    pub fn contains(&self, key: &str) -> bool {
        match self {
            Self::Object(obj) => obj.contains_key(key),
            _ => false,
        }
    }

    pub fn as_i64(&self, fallback: i64) -> i64 {
        match self {
            Self::Signed(v) => *v,
            Self::Unsigned(v) => *v as i64,
            Self::Float(v) => *v as i64,
            Self::Bool(v) => {
                if *v { 1 } else { 0 }
            }
            _ => fallback,
        }
    }

    pub fn as_u64(&self, fallback: u64) -> u64 {
        match self {
            Self::Unsigned(v) => *v,
            Self::Signed(v) if *v >= 0 => *v as u64,
            Self::Float(v) if *v >= 0.0 => *v as u64,
            Self::Bool(v) => {
                if *v { 1 } else { 0 }
            }
            _ => fallback,
        }
    }

    pub fn as_f64(&self, fallback: f64) -> f64 {
        match self {
            Self::Float(v) => *v,
            Self::Signed(v) => *v as f64,
            Self::Unsigned(v) => *v as f64,
            Self::Bool(v) => {
                if *v { 1.0 } else { 0.0 }
            }
            _ => fallback,
        }
    }

    pub fn as_bool(&self, fallback: bool) -> bool {
        match self {
            Self::Bool(v) => *v,
            Self::Signed(v) => *v != 0,
            Self::Unsigned(v) => *v != 0,
            Self::Float(v) => *v != 0.0,
            Self::String(v) => !v.is_empty(),
            Self::Array(v) => !v.is_empty(),
            Self::Object(v) => !v.is_empty(),
            _ => fallback,
        }
    }

    pub fn as_string(&self, fallback: &str) -> String {
        match self {
            Self::String(v) => v.clone(),
            Self::Bool(v) => v.to_string(),
            Self::Signed(v) => v.to_string(),
            Self::Unsigned(v) => v.to_string(),
            Self::Float(v) => v.to_string(),
            _ => fallback.to_string(),
        }
    }

    pub fn as_array(&self) -> Option<&Vec<DynVal>> {
        match self {
            Self::Array(v) => Some(v),
            _ => None,
        }
    }

    pub fn as_array_mut(&mut self) -> &mut Vec<DynVal> {
        if !matches!(self, Self::Array(_)) {
            *self = Self::Array(Vec::new());
        }
        match self {
            Self::Array(v) => v,
            _ => unreachable!(),
        }
    }

    pub fn as_object(&self) -> Option<&HashMap<String, DynVal>> {
        match self {
            Self::Object(v) => Some(v),
            _ => None,
        }
    }

    pub fn as_object_mut(&mut self) -> &mut HashMap<String, DynVal> {
        if !matches!(self, Self::Object(_)) {
            *self = Self::Object(HashMap::new());
        }
        match self {
            Self::Object(v) => v,
            _ => unreachable!(),
        }
    }

    pub fn get(&self, key: &str) -> Option<&DynVal> {
        self.as_object()?.get(key)
    }

    pub fn get_index(&self, index: usize) -> Option<&DynVal> {
        self.as_array()?.get(index)
    }

    pub fn insert(&mut self, key: impl Into<String>, value: DynVal) {
        self.as_object_mut().insert(key.into(), value);
    }

    pub fn push(&mut self, value: DynVal) {
        self.as_array_mut().push(value);
    }

    pub fn from_json(src: &JsonValue) -> Self {
        match src {
            JsonValue::Null => Self::Null,
            JsonValue::Bool(v) => Self::Bool(*v),
            JsonValue::Number(n) => {
                if let Some(v) = n.as_i64() {
                    Self::Signed(v)
                } else if let Some(v) = n.as_u64() {
                    Self::Unsigned(v)
                } else if let Some(v) = n.as_f64() {
                    Self::Float(v)
                } else {
                    Self::Null
                }
            }
            JsonValue::String(v) => Self::String(v.clone()),
            JsonValue::Array(arr) => {
                Self::Array(arr.iter().map(Self::from_json).collect())
            }
            JsonValue::Object(obj) => {
                let mapped = obj
                    .iter()
                    .map(|(k, v)| (k.clone(), Self::from_json(v)))
                    .collect();
                Self::Object(mapped)
            }
        }
    }

    pub fn to_json(&self) -> JsonValue {
        match self {
            Self::Null => JsonValue::Null,
            Self::Bool(v) => JsonValue::Bool(*v),
            Self::Signed(v) => JsonValue::from(*v),
            Self::Unsigned(v) => JsonValue::from(*v),
            Self::Float(v) => JsonValue::from(*v),
            Self::String(v) => JsonValue::from(v.clone()),
            Self::Array(arr) => JsonValue::Array(arr.iter().map(|x| x.to_json()).collect()),
            Self::Object(obj) => {
                let mut map = Map::new();
                for (k, v) in obj {
                    map.insert(k.clone(), v.to_json());
                }
                JsonValue::Object(map)
            }
        }
    }
}

impl From<()> for DynVal {
    fn from(_: ()) -> Self {
        Self::Null
    }
}

impl From<bool> for DynVal {
    fn from(v: bool) -> Self {
        Self::Bool(v)
    }
}

impl From<i8> for DynVal { fn from(v: i8) -> Self { Self::Signed(v as i64) } }
impl From<i16> for DynVal { fn from(v: i16) -> Self { Self::Signed(v as i64) } }
impl From<i32> for DynVal { fn from(v: i32) -> Self { Self::Signed(v as i64) } }
impl From<i64> for DynVal { fn from(v: i64) -> Self { Self::Signed(v) } }
impl From<isize> for DynVal { fn from(v: isize) -> Self { Self::Signed(v as i64) } }

impl From<u8> for DynVal { fn from(v: u8) -> Self { Self::Unsigned(v as u64) } }
impl From<u16> for DynVal { fn from(v: u16) -> Self { Self::Unsigned(v as u64) } }
impl From<u32> for DynVal { fn from(v: u32) -> Self { Self::Unsigned(v as u64) } }
impl From<u64> for DynVal { fn from(v: u64) -> Self { Self::Unsigned(v) } }
impl From<usize> for DynVal { fn from(v: usize) -> Self { Self::Unsigned(v as u64) } }

impl From<f32> for DynVal { fn from(v: f32) -> Self { Self::Float(v as f64) } }
impl From<f64> for DynVal { fn from(v: f64) -> Self { Self::Float(v) } }

impl From<&str> for DynVal {
    fn from(v: &str) -> Self {
        Self::String(v.to_string())
    }
}

impl From<String> for DynVal {
    fn from(v: String) -> Self {
        Self::String(v)
    }
}

impl From<Vec<DynVal>> for DynVal {
    fn from(v: Vec<DynVal>) -> Self {
        Self::Array(v)
    }
}

impl From<HashMap<String, DynVal>> for DynVal {
    fn from(v: HashMap<String, DynVal>) -> Self {
        Self::Object(v)
    }
}

impl Index<&str> for DynVal {
    type Output = DynVal;

    fn index(&self, key: &str) -> &Self::Output {
        static NULL: DynVal = DynVal::Null;

        match self {
            DynVal::Object(map) => map.get(key).unwrap_or(&NULL),
            _ => &NULL,
        }
    }
}

impl Index<usize> for DynVal {
    type Output = DynVal;

    fn index(&self, index: usize) -> &Self::Output {
        static NULL: DynVal = DynVal::Null;

        match self {
            DynVal::Array(items) => items.get(index).unwrap_or(&NULL),
            _ => &NULL,
        }
    }
}
